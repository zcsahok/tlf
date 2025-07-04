/*
 * Tlf - contest logging program for amateur radio operators
 * Copyright (C) 2001-2002-2003 Rein Couperus <pa0rct@amsat.org>
 *               2012           Thomas Beierlein <tb@forth-ev.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

/* ------------------------------------------------------------
 *   edit the 5 latest qsos
 *--------------------------------------------------------------*/


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "background_process.h"
#include "err_utils.h"
#include "globalvars.h"		// Includes glib.h and tlf.h
#include "keystroke_names.h"
#include "logview.h"
#include "readcalls.h"
#include "log_utils.h"
#include "scroll_log.h"
#include "tlf_curses.h"
#include "ui_utils.h"

#define NR_LINES 5
#define NR_COLS 80

typedef struct {
    int start;
    int end;
    bool tab;
    const char *pattern;
} field_t;

#define RST_PATTERN     "^\\s*(\\d+|-+)\\s*$"

static field_t qso_fields[] = {
    {
	.start = 0,  .end = 5, .tab = true,    // band+mode
	.pattern = "^[ 1]\\d{2}(CW |SSB|DIG)$"
    },
    {
	.start = 7,  .end = 15,                // date
	.pattern = "^\\d{2}-[A-Z]{3}-\\d{2}$"
    },
    {
	.start = 17, .end = 21, .tab = true,   // time
	.pattern = "^\\d{2}:\\d{2}$"
    },
    {
	.start = 23, .end = 26,                // number
	.pattern = "^\\s*\\d+\\s*$"
    },
    {
	.start = 29, .end = 29 + MAX_CALL_LENGTH - 1, .tab = true,   // call
	.pattern = "^\\s*\\S+\\s*$"
    },
    {
	.start = 44, .end = 46,                // sent RST
	.pattern = RST_PATTERN
    },
    {
	.start = 49, .end = 51,                // rcvd RST
	.pattern = RST_PATTERN
    },
    {.start = 54,            .tab = true},  // exchange -- end set at runtime
};

#define QSO_FIELD_INDEX_CALL        4
#define QSO_FIELD_INDEX_EXCHANGE    (G_N_ELEMENTS(qso_fields) - 1)

static field_t comment_fields[] = {
    {
	.start = 1, .end = 79, .tab = true      // the whole line except leading semicolon
    },
};

static char editbuffer[LOGLINELEN + 1];
static bool changed, needs_rescore;
static int editline;
static int field_index;
static int n_fields;
static field_t *fields;         // either qso_fields or comment_fields
static field_t *current_field;  // points to fields[field_index]
static int cursor;

/* highlight the edit line and set the cursor */
static void highlight_line(int row, char *line, int column) {

    char ln[NR_COLS + 1];

    g_strlcpy(ln, line, NR_COLS + 1);
    attron(COLOR_PAIR(C_HEADER) | A_STANDOUT);
    mvaddstr(7 + row, 0, ln);
    move(7 + row, column);
    refreshp();
}

/* reset the highlight state */
static void unhighlight_line(int row, char *line) {

    char ln[NR_COLS + 1];

    g_strlcpy(ln, line, NR_COLS + 1);
    attron(COLOR_PAIR(C_LOG) | A_STANDOUT);
    mvaddstr(7 + row, 0, ln);
}

/* flash a field */
static void flash_field(int row, int column, char *value) {
    attrset(COLOR_PAIR(C_DUPE));
    mvaddstr(7 + row, column, value);
    curs_set(0);        // hide cursor
    refreshp();
    usleep(200 * 1000); // 200 ms
    curs_set(1);        // show cursor
    // note: line will be re-displayed in the main loop
}

/*
 align cursor to a valid position in current line
 updates:
        - cursor
        - field_index
        - current_field
*/
static void align_cursor(char *buffer) {
    int new_column = 0;
    int new_field_index = 0;
    int min_distance = 999;

    // check if rigth from cursor we have only spaces
    bool spaces = true;
    for (int i = cursor + 1; buffer[i]; ++i) {
	if (buffer[i] != ' ') {
	    spaces = false;
	    break;
	}
    }
    if (spaces) {
	// then move cursor left until next non-empty position
	// (it is assumed that fields are separated by spaces)
	while (buffer[cursor] == ' ' && cursor > fields[0].start) {
	    --cursor;
	}
    }

    for (int i = 0; i < n_fields; i++) {
	// good case: column falls within a field
	if (cursor >= fields[i].start && cursor <= fields[i].end) {
	    new_column = cursor;
	    new_field_index = i;
	    break;
	}

	// else: find the closest field
	int c, d;
	if (fields[i].start > cursor) { // field is to the right
	    c = fields[i].start;
	    d = c - cursor;
	} else {
	    c = fields[i].end;
	    d = cursor - c;
	}
	if (d < min_distance) {
	    min_distance = d;
	    new_column = c;
	    new_field_index = i;
	}
    }

    cursor = new_column;
    field_index = new_field_index;
    current_field = &fields[field_index];
}


/* get a copy of selected QSO into the buffer */
static void get_qso(int nr, char *buffer) {

    assert(nr < NR_QSOS);
    strcpy(buffer, QSOS(nr));
    assert(strlen(buffer) == (LOGLINELEN - 1));

    if (log_is_comment(buffer)) {
	fields = comment_fields;
	n_fields = G_N_ELEMENTS(comment_fields);
    } else {
	fields = qso_fields;
	n_fields = G_N_ELEMENTS(qso_fields);
    }

    align_cursor(buffer);
}

/* save editbuffer back to log */
static void putback_qso(int nr, char *buffer) {
    FILE *fp;

    assert(strlen(buffer) == (LOGLINELEN - 1));
    assert(nr < NR_QSOS);

    if ((fp = fopen(logfile, "r+")) == NULL) {
	TLF_SHOW_WARN("Can not open logfile...");
    } else {
	fseek(fp, (long)nr * LOGLINELEN, SEEK_SET);
	fputs(buffer, fp);
	fputs("\n", fp);

	fclose(fp);

	struct qso_t *qso = parse_qso(buffer);
	struct qso_t *old_qso = g_ptr_array_index(qso_array, nr);
	g_ptr_array_index(qso_array, nr) = qso;
	free_qso(old_qso);
    }
}

static bool field_valid() {
    if (!changed || current_field->pattern == NULL) {
	return true;
    }

    // extract current field value
    char value[NR_COLS + 1];
    int width = current_field->end - current_field->start + 1;
    g_strlcpy(value, editbuffer + current_field->start, width + 1);

    bool valid = g_regex_match_simple(current_field->pattern, value,
				      G_REGEX_CASELESS, 0);
    if (!valid) {
	flash_field(editline, current_field->start, value);
    }
    return valid;
}


static void check_store_and_get_next_line(int direction) {
    if (!field_valid()) {
	return;     // field is not valid, no action needed
    }
    unhighlight_line(editline, editbuffer);
    if (changed) {
	putback_qso(NR_QSOS - (NR_LINES - editline), editbuffer);
	needs_rescore = true;
	changed = false;
    }
    if (direction != 0) {
	editline += direction;
	get_qso(NR_QSOS - (NR_LINES - editline), editbuffer);
    }
}


void edit_last(void) {

    int j, k;

    if (NR_QSOS == 0)
	return;			/* nothing to edit */

    pause_background_process();  // note: this freezes nr_qsos, as network is paused

    const int topline = (NR_LINES > NR_QSOS ? NR_LINES - NR_QSOS : 0);

    // set current end of exchange field
    qso_fields[QSO_FIELD_INDEX_EXCHANGE].end =
	qso_fields[QSO_FIELD_INDEX_EXCHANGE].start +
	contest->exchange_width - 1;

    /* start with last QSO or comment (note) */
    editline = NR_LINES - 1;
    changed = false;
    needs_rescore = false;

    // initial position: start of call field
    cursor = qso_fields[QSO_FIELD_INDEX_CALL].start;

    get_qso(NR_QSOS - (NR_LINES - editline), editbuffer);

    while (true) {
	highlight_line(editline, editbuffer, cursor);

	j = key_get();

	// Ctrl-A (^A) or <Home>, beginning of line.
	if (j == CTRL_A || j == KEY_HOME) {
	    if (field_valid()) {
		field_index = 0;
		current_field = &fields[field_index];
		cursor = current_field->start;
	    }

	    // Ctrl-E (^E) or <End>, end of last field.
	} else if (j == CTRL_E || j == KEY_END) {
	    if (field_valid()) {
		field_index = n_fields - 1;
		current_field = &fields[field_index];
		cursor = current_field->end;
	    }

	    // <Tab>, switch to next tab field.
	} else if (j == TAB) {
	    if (field_valid()) {
		do {
		    field_index = (field_index + 1) % n_fields;
		} while (!fields[field_index].tab);

		current_field = &fields[field_index];
		cursor = current_field->start;
	    }

	    // Up arrow, move to previous line.
	} else if (j == KEY_UP) {
	    if (editline > topline) {
		check_store_and_get_next_line(-1);
	    } else {
		if (field_valid()) {
		    logview();
		    j = ESCAPE;     // signal exit
		}
	    }

	    // Down arrow, move to next line.
	} else if (j == KEY_DOWN) {
	    if (editline < NR_LINES - 1) {
		check_store_and_get_next_line(+1);
	    } else {
		j = ESCAPE;     // signal exit
	    }

	    // Left arrow, move cursor one position left.
	} else if (j == KEY_LEFT) {
	    if (cursor > current_field->start) {
		cursor--;
	    } else if (field_valid() && field_index > 0) {
		--field_index;
		current_field = &fields[field_index];
		cursor = current_field->end;
	    }

	    // Right arrow, move cursor one position right.
	} else if (j == KEY_RIGHT) {
	    if (cursor < current_field->end) {
		cursor++;
	    } else if (field_valid() && field_index < n_fields - 1) {
		++field_index;
		current_field = &fields[field_index];
		cursor = current_field->start;
	    }

	    // <Insert>, insert a space
	} else if (j == KEY_IC) {
	    for (k = current_field->end; k > cursor; k--)
		editbuffer[k] = editbuffer[k - 1];
	    editbuffer[cursor] = ' ';
	    changed = true;

	    // <Delete>, shift rest left
	} else if (j == KEY_DC) {
	    for (k = cursor; k < current_field->end; k++)
		editbuffer[k] = editbuffer[k + 1];
	    editbuffer[current_field->end] = ' ';
	    changed = true;

	} else {

	    // Promote lower case to upper case.
	    if (j >= 'a' && j <= 'z')
		j = j - 'a' + 'A';

	    // Accept most all printable characters.
	    if (j >= ' ' && j < 'a') {
		editbuffer[cursor] = j;
		if (cursor < current_field->end) {
		    cursor++;
		}
		changed = true;
	    }
	}

	// Check exit
	if (j == ESCAPE || j == '\n' || j == KEY_ENTER) {
	    if (field_valid()) {
		break;      // exit loop
	    }
	}
    }

    check_store_and_get_next_line(0);

    if (needs_rescore) {
	log_read_n_score();
    }

    scroll_log();

    resume_background_process();
}
