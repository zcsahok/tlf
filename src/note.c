/*
 * Tlf - contest logging program for amateur radio operators
 * Copyright (C) 2001-2002-2003 Rein Couperus <pa0rct@amsat.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
   	/* ------------------------------------------------------------
 	*       Include note  in log
 	*
 	*--------------------------------------------------------------*/

#include "globalvars.h"
#include "note.h"

int include_note(void)
{

 extern char backgrnd_str[];
 extern char logfile[];
 extern char qsonrstr[];
 extern char thisnode;


 char  buffer[80]  = "";
 char  buffer2[160]  = "";

 int  i;
 FILE *fp;

	attron(A_STANDOUT);
        mvprintw(15,1, "                                                                                    ");
        nicebox(14,0, 1, 78, "Note");
        attron(A_STANDOUT);
        mvprintw(15,1,"");

     	echo();
		getnstr(buffer, 78);
		noecho();

		buffer2[0] = '\0';

		if (lan_active == 1) {
			sprintf(buffer2, "; Node %c, %d : ", thisnode, atoi(qsonrstr) - 1);
		}else
			sprintf(buffer2, "; ");

		if (strlen(buffer) >= 1){
			strncat(buffer2,  buffer,  strlen(buffer)); /* strip \n */
			strncat(buffer2, backgrnd_str, 80  -  strlen(buffer2));
			buffer2[80] = '\0';
		
			if  ( (fp = fopen(logfile,"a"))  == NULL){
				endwin();
				fprintf(stdout,  "\nnote.c: Error opening log file.\n");
				exit(1);
			}
			strcat(buffer2, "\n");
			fputs  (buffer2, fp);

			fclose(fp);
			
			scroll_log();
			strncpy(logline4,  buffer2, (strlen(buffer2)-1));
			clear_display();
			
		}
		
		attron(COLOR_PAIR(COLOR_WHITE | A_STANDOUT));
			
		for  (i=14 ; i<=16 ;  i++)
			mvprintw(i,0,  backgrnd_str);
		

		return(0);
}
