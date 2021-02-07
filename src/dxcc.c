/*
 * Tlf - contest logging program for amateur radio operators
 * Copyright (C) 2011, 2013 Thomas Beierlein <tb@forth-ev.de>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <glib.h>
#include <math.h>

#include "dxcc.h"


GPtrArray *dxcc;
GPtrArray *prefix;
static GArray *prefix_hash[36 * 36];    // 2 base36 digits
bool have_exact_matches;
char cty_dat_version[12];   // VERyyyymmdd

prefix_data dummy_pfx = {
    "No Prefix",
    0,
    0,
    0,
    0,
    INFINITY,
    INFINITY,
    NULL,
    INFINITY,
    false
};

void prefix_free(gpointer data) {
    prefix_data *pfx_data = data;

    g_free(pfx_data -> pfx);
    g_free(pfx_data -> continent);
    g_free(pfx_data);

    for (int i = 0; i < G_N_ELEMENTS(prefix_hash); ++i) {
	if (prefix_hash[i] != NULL) {
	    g_array_free(prefix_hash[i], TRUE);
	    prefix_hash[i] = NULL;
	}
    }
}


void prefix_init(void) {
    if (prefix) {
	g_ptr_array_free(prefix, TRUE);
    }
    prefix = g_ptr_array_new_with_free_func(prefix_free);

    for (int i = 0; i < G_N_ELEMENTS(prefix_hash); ++i) {
	prefix_hash[i] = NULL;
    }

    have_exact_matches = false;
}

/* return number of entries in prefix array */
unsigned int prefix_count(void) {
    return prefix->len;
}

/* give pointer to prefix struct at 'index' */
prefix_data *prefix_by_index(unsigned int index) {
    if (index >= prefix_count())
	return &dummy_pfx;

    return (prefix_data *)g_ptr_array_index(prefix, index);
}

/* convert char to base36 */
int to_base36(char c) {
    if (isdigit(c)) {
	return c - '0';
    }
    if (isupper(c)) {
	return 10 + c - 'A';
    }
    return 0;   // rest is mapped to zero
}

/* get hash key for a call/prefix */
int prefix_hash_key(const char *call) {
    if (call[0] == 0) { // normally call is never empty
	return 0;
    }
    return to_base36(call[0]) + 36 * to_base36(call[1]);
}

/* get count of indices for hash key */
int prefix_count_by_key(int key) {
    if (prefix_hash[key] == NULL) {
	return 0;
    }
    return prefix_hash[key]->len;
}

/* get prefix index from hash by key+offset */
unsigned int prefix_by_key(int key, unsigned int offset) {
    if (prefix_hash[key] == NULL || offset >= prefix_hash[key]->len) {
	return 0;
    }
    int *indices = (int *)prefix_hash[key]->data;
    return indices[offset];
}

/* add index to the prefix hash */
static void prefix_hash_add(char *pfx, int index) {
    int key = prefix_hash_key(pfx);
    if (prefix_hash[key] == NULL) {
	prefix_hash[key] = g_array_new(FALSE, FALSE, sizeof(int));
    }
    g_array_append_val(prefix_hash[key], index);
}

/* add a new prefix description */
void prefix_add(char *pfxstr) {

    char *ver = (*pfxstr == '=' ? pfxstr + 1 : pfxstr);
    if (strlen(ver) == 11 && strncmp(ver, "VER", 3) == 0) {
	strcpy(cty_dat_version, ver);    // save it
    }

    gchar *loc;
    gint last_index = dxcc_count() - 1;
    dxcc_data *last_dx = dxcc_by_index(last_index);
    prefix_data *new_prefix = g_new(prefix_data, 1);

    if (*pfxstr == '=') {
	new_prefix -> exact = true;
	have_exact_matches = true;
	pfxstr++;
    } else
	new_prefix -> exact = false;

    loc = strchr(pfxstr, '~');
    if (loc != NULL) {
	new_prefix -> timezone = atof(loc + 1);
	*loc = '\0';
    } else
	new_prefix -> timezone = INFINITY;

    loc = strchr(pfxstr, '{');
    if (loc != NULL) {
	new_prefix -> continent = g_strdup(loc + 1);
	*loc = '\0';
	loc = strchr(new_prefix -> continent, '}');
	if (loc != NULL)
	    *loc = '\0';
    } else
	new_prefix -> continent = NULL;

    loc = strchr(pfxstr, '<');
    if (loc != NULL) {
	new_prefix -> lat = atof(loc + 1);
	*loc = '\0';
	if ((loc = strchr(loc, '/')) != NULL)
	    new_prefix -> lon = atof(loc + 1);
	else
	    new_prefix -> lon = INFINITY;
    } else
	new_prefix -> lat = new_prefix -> lon = INFINITY;

    loc = strchr(pfxstr, '[');
    if (loc != NULL) {
	new_prefix -> itu = atoi(loc + 1);
	*loc = '\0';
    } else
	new_prefix -> itu = last_dx -> itu;

    loc = strchr(pfxstr, '(');
    if (loc != NULL) {
	new_prefix -> cq = atoi(loc + 1);
	*loc = '\0';
    } else
	new_prefix -> cq = last_dx -> cq;

    new_prefix -> pfx = g_strdup(pfxstr);
    new_prefix -> pfx_len = strlen(pfxstr);
    new_prefix -> dxcc_index = last_index;

    g_ptr_array_add(prefix, new_prefix);
    prefix_hash_add(pfxstr, prefix_count() - 1);
}



void dxcc_free(gpointer data) {
    dxcc_data *dxcc = data;

    g_free(dxcc -> countryname);
    g_free(dxcc -> continent);
    g_free(dxcc -> pfx);
    g_free(dxcc);
}

void dxcc_init(void) {
    cty_dat_version[0] = 0;
    if (dxcc) {
	g_ptr_array_free(dxcc, TRUE);
    }
    dxcc = g_ptr_array_new_with_free_func(dxcc_free);
}

/* return number of entries in dxcc array */
unsigned int dxcc_count(void) {
    return dxcc->len;
}

/* give pointer to dxcc_data struct at 'index' */
dxcc_data *dxcc_by_index(unsigned int index) {
    if (index >= dxcc_count())
	index = 0;

    return (dxcc_data *)g_ptr_array_index(dxcc, index);
}

void dxcc_add(char *dxcc_line) {
    gchar **split;
    gint item;
    dxcc_data *new_dxcc = g_new(dxcc_data, 1);

    /* split up the line */
    split = g_strsplit(dxcc_line, ":", 9);

    if (g_strv_length(split) < 8) {	/* wrong syntax, ignore line */
	g_strfreev(split);
	g_free(new_dxcc);
	return;
    }

    for (item = 0; item < 8; item++)
	g_strstrip(split[item]);

    new_dxcc -> countryname = g_strdup(split[0]);
    new_dxcc -> cq = atoi(split[1]);
    new_dxcc -> itu = atoi(split[2]);
    new_dxcc -> continent = g_strdup(split[3]);
    new_dxcc -> lat = atof(split[4]);
    new_dxcc -> lon = atof(split[5]);
    new_dxcc -> timezone = atof(split[6]);
    if (*split[7] == '*') {
	new_dxcc -> pfx = g_strdup(split[7] + 1);
	new_dxcc -> starred = true;
    } else {
	new_dxcc -> pfx = g_strdup(split[7]);
	new_dxcc -> starred = false;
    }

    g_strfreev(split);

    g_ptr_array_add(dxcc, new_dxcc);
}

/** load cty database from filename */
int load_ctydata(char *filename) {
    FILE *fd;
    char buf[181] = "";
    char *loc;

    if ((fd = fopen(filename, "r")) == NULL)
	return -1;

    dxcc_init();
    prefix_init();

    // set default for empty country
    dxcc_add("Not Specified        :    --:  --:  --:  -00.00:    00.00:     0.0:     :");

    while (fgets(buf, sizeof(buf), fd) != NULL) {

	g_strchomp(buf); 	/* drop CR and/or NL and */
	if (*buf == '\0')	/* ignore empty lines */
	    continue;

	if (buf[0] != ' ') {	// data line

	    dxcc_add(buf);

	} else {		// prefix line
	    loc = strtok(buf, " ,;");
	    while (loc != NULL) {

		prefix_add(loc);

		loc = strtok(NULL, " ,;");
	    }
	}
    }
    fclose(fd);
    return 0;
}


