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
 	*
 	*              Update time
 	*
 	*              also updates windows every second
 	*--------------------------------------------------------------*/
#include "globalvars.h"
#include "time_update.h"

int time_update(void)
{
extern struct tm *time_ptr;
extern int searchflg;
extern char hiscall[];
extern char qsonrstr[];
extern int oldtens;
extern int bandinx;
extern int this_second;
extern long system_secs;
extern int miniterm;

char time_buf[20];
int tens  = 1;
int j;
int currentterm = 0;
static int s;
static int m;



		usleep(1000);

         	
          	get_time();
			strftime(time_buf, 10, "%H:%M:%S",  time_ptr);
			tens = time_buf[7];        /* seconds */
			this_second = tens;
			system_secs= time_buf[3]*10*60 + time_buf[4]  * 60 + time_buf[6] * 10 + time_buf[7]  ;
			time_buf[5] = '\0';

			if (tens != oldtens ){    /*  seconds */
				oldtens  = tens;

				if (wpx == 1) {
					if (minute_timer > 0) minute_timer-- ;
				}

				if (s >= 0) {
					
					if ((time_buf[6] == '1') && (m >= 30)) {
						
					    m = 0;
					    getwwv();
						
					} else {
						m++;
					}
					
					currentterm = miniterm;
					miniterm = 0;

					clusterinfo(time_buf);  /* update cluster info (2 seconds)*/

                    attron(COLOR_PAIR(7) | A_STANDOUT);

					if ((strlen(hiscall) <= 1) || (searchflg == 0)) {
						mvprintw(7,0, logline0);
						mvprintw(8,0, logline1);
						mvprintw(13,0, "                                                              ");
					}
					mvprintw(9,0, logline2);
					mvprintw(10,0, logline3);
					mvprintw(11,0, logline4);
					attron(COLOR_PAIR(COLOR_CYAN));
					mvprintw(12, 23, qsonrstr);
					printcall();					
					
					
					showscore();			/* update  score  window every 2 seconds */
					show_zones(bandinx);
					s = 0;
					miniterm = currentterm;
				}else s++;
				
				
			currentterm = miniterm;
			miniterm = 0;

			if (searchflg  == SEARCHWINDOW ){   /*    check routine erase  */
					
					attron(COLOR_PAIR(7) | A_STANDOUT);

				if  (strlen(hiscall)  < 2){
						for (j =  1;  j <= 8; j++){
							if (j == 6) {
								attron(A_STANDOUT);
								mvhline(j,  41,  ACS_HLINE, 39);		
								}
							else
								mvprintw(j, 41, "                                       ");	
						}
							mvprintw(7,0, logline0);
							mvprintw(8,0, logline1);
				}
							printcall();
			}
			miniterm = currentterm;
		}



	return(0);
}
