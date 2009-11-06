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
 	*          parameterdialog
 	*--------------------------------------------------------------*/

#include "changepars.h"
#include "sendbuf.h"
#include <termios.h>

int debug_tty (void);

int changepars(void){

extern int use_rxvt;
extern int cluster;
extern int shortqsonr;
extern int searchflg;
extern int demode;
extern int contest;
extern int announcefilter;
extern int showscore_flag;
extern int zonedisplay;
extern int trxmode;
extern char hiscall[];
extern int rit;
extern int trx_control;
extern int editor;
extern int packetinterface;
extern int cqdelay;
extern int ctcomp;
extern SCREEN *mainscreen;
extern char config_file[];
extern int miniterm;
extern char buffer[];

#ifdef HAVE_LIBHAMLIB
extern freq_t outfreq;
#else
extern int outfreq;
#endif

extern int simulator;
extern int keyerport;
extern char synclogfile[];
extern int sc_sidetone;
extern char sc_volume[];
extern int cwstart;

char  parameterstring[20];
char parameters[51][19];
char cmdstring[80];
int i, k, x, nopar  = 0;
int maxpar  = 50;
int volumebuffer;
int currentmode = 0;

strcpy(parameters[0] ,  "SPOT");
strcpy(parameters[1] ,  "MAP");
strcpy(parameters[2] ,  "CLOFF");
strcpy(parameters[3] ,  "CLUSTER");
strcpy(parameters[4] ,  "SHORT");
strcpy(parameters[5] ,  "LONG");
strcpy(parameters[6] ,  "MESSAGE");
strcpy(parameters[7] ,  "LIST");
strcpy(parameters[8] ,  "CHECK");
strcpy(parameters[9] ,  "NOCHECK");
strcpy(parameters[10],  "TONE");
strcpy(parameters[11],  "EDIT");
strcpy(parameters[12],  "VIEW");
strcpy(parameters[13],  "HELP");
strcpy(parameters[14],  "DEMODE");
strcpy(parameters[15],  "CONTEST");
strcpy(parameters[16],  "FILTER");
strcpy(parameters[17],  "SCORE");
strcpy(parameters[18],  "WRITE");
strcpy(parameters[19],  "EXIT");
strcpy(parameters[20],  "TXFILE");
strcpy(parameters[21],  "ZONES");
strcpy(parameters[22],  "CTY");
strcpy(parameters[23],  "MODE");
strcpy(parameters[24],  "SET");
strcpy(parameters[25],  "MULTI");
strcpy(parameters[26],  "PROP");
strcpy(parameters[27],  "RITCLEAR");
strcpy(parameters[28],  "TRXCONTROL");
strcpy(parameters[29],  "CFG");
//strcpy(parameters[30],  "CWMODE");
strcpy(parameters[30],  "CW");
strcpy(parameters[31],  "SSBMODE");
strcpy(parameters[32],  "DIGIMODE");
strcpy(parameters[33],  "PACKET");
strcpy(parameters[34],  "SIMULATOR");
strcpy(parameters[35],  "INFO");
strcpy(parameters[36],  "FREQ");
strcpy(parameters[37],  "RECONNECT");
strcpy(parameters[38],  "QUIT");
strcpy(parameters[39],  "CQDELAY");
strcpy(parameters[40],  "ADIF");
strcpy(parameters[41],  "SYNC");
strcpy(parameters[42],  "RESCORE");
strcpy(parameters[43],  "SCVOLUME");
strcpy(parameters[44],  "SCAN");
strcpy(parameters[45],  "DEBUG");
strcpy(parameters[46],  "MINITERM");
strcpy(parameters[47],  "RTTY");
strcpy(parameters[48], "SOUND");
strcpy(parameters[49],  "CWMODE");
strcpy(parameters[50],  "CHARS");

	nopar = 0;
	
	attroff(A_STANDOUT);
	attron(COLOR_PAIR(COLOR_GREEN));
	mvprintw(12,29,  "PARAMETER?  ");
	refresh();
	
	mvprintw(12,29, "            ");
	mvprintw(12, 29, "");
	refresh();
	
	echo();
	getstr(parameterstring);
    noecho();


    for (k = 0;  parameterstring[k]; k++)
     	parameterstring[k]  = toupper(parameterstring[k]);

	
	for (i=0 ; i <=  maxpar ; i++){
		if  (strncmp(parameterstring, parameters[i],  3) == 0){
		 	break;
		}
	}
	
	switch(i)
	{
		case 0:     		/* SPOTS) */
		{
			cluster = SPOTS;
			break;
		}
		case 1:             /* BANDMAP */
		{
			cluster = MAP;
			break;
		}
		case 2:             /* CLOFF  */
		{
			cluster = NOCLUSTER;
			break;
		}
		case 3:				/* CLUSTER  */
		{
			cluster = CLUSTER;
			announcefilter = FILTER_ALL;
			break;
		}
		case 4:				/* SHORTNR  */
		{
			shortqsonr = SHORTCW;
			break;
		}
		case 5:				/* LONGNR  */
		{
			shortqsonr = LONGCW;
			break;
		}
		case 6:				/* MESSAGE  */
		{
			message_change(i);
			break;
		}		

		case 7:				/* LIST  */
		{
			listmessages();
			break;
		}	
		case 8:				/* CHECK  */
		{
				searchflg = SEARCHWINDOW ;
			break;
		}		
		case 9:				/* NOCHECK  */
		{
				searchflg = 0 ;
			break;	
		}				
		case  10:			/*  TONE   */
		{
		 		set_tone();
			break;
		}
		case  11:			/*  EDIT   */
		{
			logedit();
			break;
		}
		case  12:			/*  VIEW   */
		{
			logview();
			break;
		}
		case  13:			/*  HELP   */
		{
//			show_help();
			 		endwin();
			 		system ("clear");
					system("less help.txt");
			 		system ("clear");
					set_term(mainscreen);
					clear_display();
			break;
		}
		case  14:			/*  DEMODE   */
		{
			if  (demode == SEND_DE )
				demode = 0;
			else
				demode = SEND_DE;
			mvprintw(13, 29,  "DE-mode is %d", demode);
			refresh();
			sleep(1);
			
			break;
		}
		case  15:			/*  CONTEST   */
		{
			if  (contest == CONTEST)
				contest = 0;
			else {
				contest = CONTEST;
				searchflg  = SEARCHWINDOW;
				}
			mvprintw(13, 29,  "CONTEST-mode is %d", contest);
			refresh();
			sleep(1);
		
			break;
		}
		case  16:			/*  FILTER   */
		{
			announcefilter++;
			if (announcefilter >3 ) announcefilter =0;
			mvprintw(13, 29,  "FILTER-mode is %d", announcefilter);
			refresh();
			sleep(1);

			break;
		}
		case  17:			/*  SCORE   */
		{
			if  (showscore_flag == 0)
				showscore_flag = 1;
			else {
				showscore_flag  = 0;
				
				}
			mvprintw(13, 29,  "Show score-mode is %d", showscore_flag);
			refresh();
			sleep(1);
		
			break;
		}
		case  18:			/*  WRITE CABRILLO FILE   */
		{
			write_cabrillo();
			
			mvprintw(13, 29,  "writing  cabrillo file");
			refresh();
			sleep(1);

			break;
		}
		case 19:     		/* EXIT */
		{
			writeparas();
			clear();
           		endwin();
 			puts("\n\nThanks for using TLF.. 73\n");
			exit(0);
			break;
		}
		case  20:			/*  TXFILE   */
		{
			break;
		}
		case  21:			/*  ZONES   */
		{
			if  (zonedisplay == 0)
				zonedisplay = 1;
			else {
				zonedisplay  = 0;
				
				}

			break;
		}
		case 22:            /* COUNTRIES */
		{
			show_mults();
			refresh();
			sleep(1);
		
			break;
		}
		case  23:			/*  MODE   */
		{
			if  (trxmode == CWMODE)
				trxmode = SSBMODE;
			else if (trxmode == SSBMODE)
				trxmode = DIGIMODE;
			else
				trxmode = CWMODE;

			if (trxmode == CWMODE){
				mvprintw(13, 29,  "TRXMODE = CW");
			}else if (trxmode == SSBMODE)
				mvprintw(13, 29,  "TRXMODE = SSB");
			else
				mvprintw(13, 29,  "TRXMODE = DIG");
			refresh();
			sleep(1);
		
			break;
		}
  		case 24:            /* SET PARAMETERS */
		{
			clear();
			if (editor == EDITOR_JOE){
				strcpy (cmdstring, "joe ");
				strcat (cmdstring, config_file);
				system(cmdstring);
				}
			else if (editor == EDITOR_VI){
				strcpy (cmdstring, "vi ");
				strcat (cmdstring, config_file);
				system(cmdstring);
				}
			else if (editor == EDITOR_MC){
				strcpy (cmdstring, "mcedit ");
				strcat (cmdstring, config_file);
				system(cmdstring);
				}
			else
{
				strcpy (cmdstring, "e3 ");
				strcat (cmdstring, config_file);
				system(cmdstring);
				}

			read_logcfg();
			writeparas();
			mvprintw(24,0, "Logcfg.dat loaded, parameters written..");
			refresh();
		    clear_display();
			break;
		}
         		case  25:			/*  MULTI   */
		{
			multiplierinfo();

			break;
		}
			case 26:            /* CONDX */
		{
			muf();
		    clear_display();
			break;
		}
  			case  27:			/*  RITCLEAR   */
		{
				if  (rit == RITCLEAR)
					rit = 0;
				else {
					rit = RITCLEAR;
				
				}
				if (rit == RITCLEAR){
					mvprintw(13, 29,  "RIT clear on");
				}else{
					mvprintw(13, 29,  "RIT clear off");
				}
			refresh();
			sleep(1);
		
			break;
		}
  		case  28:			/*  trx ctl   */
		{
				if  (trx_control == 1)
					trx_control = 0;
				else {
					trx_control = 1;

				}
				if (trx_control == 1){
					mvprintw(13, 29,  "TRX control on");
				}else{
					mvprintw(13, 29,  "TRX control off");
				}
			refresh();
			sleep(1);

			break;
		}
		case 29:            /* LOAD PARAMETERS */
		{
			clear();
			if (editor == EDITOR_JOE){
				strcpy (cmdstring, "joe ");
				strcat (cmdstring, config_file);
				system(cmdstring);
				}
			else if (editor == EDITOR_VI){
				strcpy (cmdstring, "vi ");
				strcat (cmdstring, config_file);
				system(cmdstring);
				}
			else if (editor == EDITOR_MC){
				strcpy (cmdstring, "mcedit ");
				strcat (cmdstring, config_file);
				system(cmdstring);
				}
			else
{
				strcpy (cmdstring, "e3 ");
				strcat (cmdstring, config_file);
				system(cmdstring);
				}
			read_logcfg();
			writeparas();
			mvprintw(24,0, "Logcfg.dat loaded, parameters written..");
			refresh();
		    clear_display();
			break;
		}
		case 30:				/* CW  */
		case 49:
		{
		if (keyerport == MFJ1278_KEYER) {
			strcpy (buffer, "MODE CW,30");
			buffer[7] =  '\015';
			buffer[8] =  'K';
			buffer[9] =  '\015';
			buffer[10] = '\0';
			sendbuf();
		}
			trxmode = CWMODE;

			if(trx_control == 1)
				outfreq =  SETCWMODE;
			break;
		}
		case 31:				/* SHORTNR  */
		{
			trxmode = SSBMODE;
				outfreq = SETSSBMODE;
			break;
		}
		case 32:				/* SHORTNR  */
		{
			trxmode = DIGIMODE;
			break;
		}
		case 33:				/* PACKET  */
		{
			if (packetinterface > 0) packet();
			break;
		}
		case 34:				/* SIMULATOR  */
		{
			if (simulator == 0){
				simulator=1;
				if (ctcomp == 1) {
					mvprintw(13,19, "The simulator only works in TRmode. Switching to TRmode");
					ctcomp=0;
				}
				else
					mvprintw(13, 29, "Simulator on");

				refresh();

				if (keyerport == NET_KEYER){

					if (netkeyer (K_WORDMODE, NULL) < 0){
						mvprintw(24,0, "keyer not active; switching to SSB");
						trxmode = SSBMODE;
						clear_display();
					}
				}
			}  else {
				simulator=0;
				mvprintw(13, 29, "Simulator off");
				refresh();

				if (keyerport == NET_KEYER){

					if (netkeyer (K_RESET, NULL) < 0){
						mvprintw(24,0, "keyer not active; switching to SSB");
						trxmode = SSBMODE;
						clear_display();
					}				
				}

			}
			break;
		}
		case 35:				/* INFO  */
		{

			miniterm = 0;
			networkinfo();
			miniterm = 1;

			if (currentmode == DIGIMODE)
				trxmode = DIGIMODE;
			break;
		}
		case 36:             /* CLOFF  */
		{
			cluster = FREQWINDOW;
			break;
		}

		case 37:				/* RECONNECT  */
		{
			if (packetinterface > 0) {
				cleanup_telnet();
				init_packet();
				packet();
			}
			break;
		}

		case 38:     		/* EXIT=QUIT */
		{
			writeparas();
			endwin();
 			puts("\n\nThanks for using TLF.. 73\n");
             exit(0);
			break;
		}
		case 39 :
		{
			mvprintw(12,29, "CQD: pgup/dwn", cqdelay);
			refresh();
			
			x = 1;
			while (x) {
				x=onechar();

				switch (x) {
					case 156: {
						if  (cqdelay <= 60){
							cqdelay++;
							attron(COLOR_PAIR(COLOR_GREEN) | A_STANDOUT);
		 					mvprintw(0, 19, "  ");
		 					mvprintw(0, 19, "%i",  cqdelay);
							break;

						}
					}
					case 157: {
						if (cqdelay >= 1){
				 			cqdelay--;
				 			attron(COLOR_PAIR(COLOR_GREEN) | A_STANDOUT);

		 					mvprintw(0, 19, "  ");
		 					mvprintw(0, 19, "%i",  cqdelay);
							break;

						}
					default:
						x = 0;
					}

				}
			}

			if (use_rxvt == 0) attron(COLOR_PAIR(NORMCOLOR)  | A_BOLD );
			else attron(COLOR_PAIR(NORMCOLOR) );

			mvprintw(12, 29 +  strlen(hiscall)  , "");
			break;


		}
		case 40: {
			write_adif();
			mvprintw(13, 29,  "writing adif file");
			refresh();
			sleep(1);

			break;
		}
			case 41:            /* SYNC */
		{
			if (strlen(synclogfile) > 0)
				synclog(synclogfile);
			scroll_log();
			readcalls();
		    clear_display();
			break;
		}
			case 42:            /* RESCORE */
		{
			readcalls();
		    clear_display();
			break;
		}
			case 43:            /* SCVOLUME */		/* set soundcard volume */
		{
			if (sc_sidetone == 0) break;

			mvprintw(12, 29, "Vol: %d", atoi(sc_volume));
			volumebuffer = atoi(sc_volume);

			x = 1;
			while (x)
			{
				x=onechar();

				switch (x) {
					case 156: {
						if  (volumebuffer <= 99){
							volumebuffer +=5;
							attron(COLOR_PAIR(COLOR_GREEN) | A_STANDOUT);
		 					mvprintw(12, 34, "  ");
		 					mvprintw(12, 34, "%d",  volumebuffer);
							break;

						}
					}
					case 157: {
						if (volumebuffer >= 6)
						{
				 			volumebuffer -= 5;
				 			attron(COLOR_PAIR(COLOR_GREEN) | A_STANDOUT);

		 					mvprintw(12, 34, "  ");
		 					mvprintw(12, 34, "%d",  volumebuffer);
							break;

						}
					default:
						x = 0;
					}
					if (volumebuffer >= 0 && volumebuffer <=99)
						sprintf(sc_volume, "%d", volumebuffer);

				}
				netkeyer (K_STVOLUME, sc_volume);
			}


		    clear_display();
			break;
		}
		case 44:            /* SCAN */
		{
			miniterm = 0;
			testaudio();
			clear_display();
		    	miniterm = 1;
			break;
		}
		case 45:            /* DEBUG */
		{
			debug_tty();
		    clear_display();
			break;
		}
		case 46:		/* MINITERM ON/OFF */
		{
			if (miniterm == 1)
				miniterm = 0;
			else
				miniterm = 1;
			break;
		}
		case 47:		/* Initialize RTTY mode  (MFJ1278B controller)*/
		{
			strcpy (buffer, "MODE VB");
			buffer[7] =  '\015';
			buffer[8] =  'K';
			buffer[9] =  '\015';
			buffer[10] = '\0';
			sendbuf();
			trxmode=DIGIMODE;

			break;
		}
		case 48:            /* SOUND */
		{
			clear_display();
			record();
			clear_display();
			break;
		}
		case 50:
		{
			mvprintw(13, 29, "? Characters: (0...4)");
			refresh();
			x = onechar();
			if ((x - 48) < 5 && (x - 48) >= 1)
				cwstart = x - 48;
			if ( (x-48) == 0) cwstart = 0;
			
			break;
				
		}
		default:
		{
		
		nopar = 1;
		
		}
	}
	
	if (nopar != 1){
		mvprintw(12, 29, "OK !        ");
		writeparas();
		}
	else{
		if (packetinterface > 0) packet();

	}
	
	refresh();

	if (use_rxvt == 0) attron(COLOR_PAIR(NORMCOLOR) | A_BOLD   );
	else   attron(COLOR_PAIR(NORMCOLOR) );
	
	mvprintw(12, 29, "            ");
	mvprintw(12, 29, "");
	refresh();
	hiscall[0] = '\0';


 return(0);
 }

/* -------------------------------------------------------------- */

int networkinfo(void){

extern int use_rxvt;
extern int use_bandoutput;
extern int recv_packets;
extern int recv_error;
extern int send_packets[];
extern int send_error[];
extern int lan_active;
extern int nodes;
extern char bc_hostaddress[MAXNODES][16];
extern char config_file[];
extern char whichcontest[];
extern char pr_hostaddress[];
extern char tncportname[];
extern char rigportname[];
extern char logfile[];

int i, j, inode, key;

		clear();

		if (use_rxvt == 0) attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD | A_STANDOUT);
		else  attron(COLOR_PAIR(COLOR_CYAN) | A_STANDOUT);

		for (j = 0; j <= 24; j++)
  			mvprintw(j,0,"                                                                                ");

	  	if (lan_active == 1)
	  		mvprintw(1,10, "Network status: on");
	  	else
	  		mvprintw(1,10, "Network status: off");

		mvprintw(3,28, "Packets rcvd: %d | %d", recv_packets, recv_error);

  		for (inode = 0; inode < nodes; inode++) {
  			mvprintw(4 + inode,10, "%s", bc_hostaddress[inode]);
  			mvprintw(4 + inode, 28, "Packets sent: %d | %d ", send_packets[inode], send_error[inode], nodes);
	  	}
		
		if (strlen(config_file) > 0) mvprintw(6 + inode, 10, "Config file: %s", config_file);
		else mvprintw(6 + inode, 10, "Config file: /usr/local/share/tlf/logcfg.dat");
		mvprintw(7 + inode, 10, "Contest    : %s", whichcontest);
		mvprintw(8 + inode, 10, "Logfile    : %s", logfile);

		mvprintw(9 + inode, 10, "Cluster    : %s", pr_hostaddress);
		mvprintw(10 + inode, 10, "TNCport    : %s", tncportname);
		mvprintw(11 + inode, 10, "RIGport    : %s", rigportname);
		if (use_bandoutput == 1) mvprintw(12 + inode, 10, "Band output: on");
		else mvprintw(12 + inode, 10, "Band output: off");


	  	refresh();

	  	mvprintw(23,22," --- Press a key to continue --- ");
	  	refresh();

	  	key = getch();

	  	if (use_rxvt == 0) attron(COLOR_PAIR(COLOR_WHITE) | A_BOLD | A_STANDOUT);
		else    attron(COLOR_PAIR(COLOR_WHITE)  | A_STANDOUT);
	 	for (i = 0; i <= 24; i++)
		 	mvprintw(i,0,"                                                                                ");

		clear_display();

return(0);

}

/* -------------------------------------------------------------- */

int multiplierinfo(void){

extern int use_rxvt;
extern int arrlss;
extern int serial_section_mult;
extern int sectn_mult;
extern char mults[MAX_MULTS][12];
extern int mult_bands[MAX_MULTS];
extern char mults_possible[MAX_MULTS][12];
extern int multcount;

int j, k,key, vert, hor, cnt, found;
char mprint[50];
char chmult[4];
char ch2mult[4];

		clear();

		if (use_rxvt == 0) attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD | A_STANDOUT);
		else   attron(COLOR_PAIR(COLOR_CYAN)  | A_STANDOUT);

		for (j = 0; j <= 24; j++)
  			mvprintw(j,0,"                                                                                ");

		if (arrlss == 1) {
			mvprintw(2, 20, "ARRL SWEEPSTAKES -- REMAINING SECTIONS");
			cnt=0;
			for (vert = 9;  vert < 18; vert++) {

				for (hor = 5; hor < 15;  hor++){
					mprint[0]='\0';
					strcat(mprint, mults_possible[cnt]);
					strcat (mprint, " ");
					mprint[4]='\0';

					found = 0;
					for (j = 0; j < multcount+1; j++) {
						strcpy (chmult, mults_possible[cnt]);
						strcpy (ch2mult, mults[j]);
						
						if (ch2mult[2]== ' ') ch2mult[2] = '\0';


						if (strcmp(ch2mult, chmult) == 0)
							found = 1;
					}
					if (found == 1)

							if (use_rxvt == 0) attron(COLOR_PAIR(COLOR_GREEN) | A_BOLD | A_STANDOUT);
							else  attron(COLOR_PAIR(COLOR_GREEN) |  A_STANDOUT);

					 else
							if (use_rxvt == 0) attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD | A_STANDOUT);
							else   attron(COLOR_PAIR(COLOR_CYAN) | A_STANDOUT);

					if ((strlen(mprint) >1) && (strcmp (mprint, "W ") != 0)) mvprintw(vert, hor*4, "%s",mprint);

					cnt++;

					if (cnt >= MAX_MULTS)  break;
				}
				if (cnt >= MAX_MULTS)  break;
			}
		}

		if (serial_section_mult == 1|| (sectn_mult == 1 && arrlss != 1)) {
			mvprintw(0, 30, "REMAINING SECTIONS");
			cnt=0;
			for (vert = 2;  vert < 22; vert++) {

				for (hor = 0; hor < 7;  hor++){
					mprint[0]='\0';
					strcat(mprint, mults_possible[cnt]);
					if (strlen(mprint) == 0 ) break;
					if (strlen(mprint) == 1 ) strcat (mprint, "   ");
					if (strlen(mprint) == 2 ) strcat (mprint, "  ");
					if (strlen(mprint) == 3 ) strcat (mprint, " ");
					if (strlen(mprint) > 3) mprint[4] = '\0';

					for (k=1 ; k <= MAX_MULTS; k++) {
						if (strstr(mults[k], mults_possible[cnt]) != NULL) break;
					}

					if ((mult_bands[k] &  BAND160 ) != 0) strcat (mprint, "*");
					else strcat(mprint, "-");
					if ((mult_bands[k] &  BAND80 ) != 0) strcat (mprint, "*");
					else strcat(mprint, "-");
					if ((mult_bands[k] &  BAND40 ) != 0) strcat (mprint, "*");
					else strcat(mprint, "-");
					if ((mult_bands[k] &  BAND20 ) != 0) strcat (mprint, "*");
					else strcat(mprint, "-");
					if ((mult_bands[k] &  BAND15 ) != 0) strcat (mprint, "*");
					else strcat(mprint, "-");
					if ((mult_bands[k] &  BAND10 ) != 0) strcat (mprint, "*");
					else strcat(mprint, "-");

					mprint[11]='\0';
					mvprintw(vert, 2 + hor*11, "%s",mprint);
					cnt++;

				}
				if (cnt >= MAX_MULTS)  break;
			}
		}


		if(use_rxvt == 0) attron(COLOR_PAIR(COLOR_CYAN) | A_BOLD | A_STANDOUT);
		else    attron(COLOR_PAIR(COLOR_CYAN) |  A_STANDOUT);

		mvprintw(23,22," --- Press a key to continue --- ");

		refresh();

	  	key = getch();

	  	if (use_rxvt == 0) attron(COLOR_PAIR(COLOR_WHITE) | A_BOLD | A_STANDOUT);
		else  attron(COLOR_PAIR(COLOR_WHITE) | A_STANDOUT);

		for (j = 0; j <= 24; j++)
		 	mvprintw(j,0,"                                                                                ");

		clear_display();

return(0);

}

/* ----------------------------------- radio link debug --------------------------------- */

int debug_tty (void) {

extern char rigportname[];
extern int serial_rate;

int fdSertnc;
int tncport = 0;
int i;
struct termios termattribs;
char line[20] = "?AF\015";
char inputline[80] = "";
const char eom[2] = {'\015' , '\0'};

/* initialize ttyS0*/

		for (i = 0; i < 24; i++)
			mvprintw(i,0, "                                                                                ");
		refresh();

		if (rigportname[strlen(rigportname) - 1] == '\n')
				rigportname[strlen(rigportname) -1 ] = '\0'; // remove \n

                mvprintw(4,0, "Trying to open %s ", rigportname);
		refresh();

		if (tncport == 1) {
    			if ((fdSertnc = open("/dev/ttyS2", O_RDWR | O_NONBLOCK)) < 0) {
                		mvprintw(5,0, "open of /dev/ttyS2 failed!!!");
				refresh();
				sleep(2);
                		return(-1);
    			}
 		}else if(tncport == 2){

   			if ((fdSertnc = open("/dev/ttyS1", O_RDWR | O_NONBLOCK)) < 0) {
                		mvprintw(5,0,  "open of /dev/ttyS1 failed!!!");
				refresh();
				sleep(2);
				return(-1);
    			}
    		}
		else {
   			if ((fdSertnc = open(rigportname, O_RDWR | O_NONBLOCK)) < 0) {
                		mvprintw(5,0,  "open of %s failed!!!", rigportname);
				refresh();
				sleep(2);
				return(-1);
    			}

		}



	termattribs.c_iflag = IGNBRK | IGNPAR | IMAXBEL | IXOFF;
    	termattribs.c_oflag = 0;
   	termattribs.c_cflag = CS8 | CSTOPB | CREAD | CLOCAL;


    	termattribs.c_lflag = 0;		    /* Set some term flags */

    	/*  The ensure there are no read timeouts (possibly writes?) */
    	termattribs.c_cc[VMIN] = 1;
    	termattribs.c_cc[VTIME] = 0;

	switch(serial_rate) {

 			case   1200 : {
				cfsetispeed(&termattribs,B1200);	    /* Set input speed */
    				cfsetospeed(&termattribs,B1200);	    /* Set output speed */
			 break;
			}

			case   2400 : {
				cfsetispeed(&termattribs,B2400);	    /* Set input speed */
    				cfsetospeed(&termattribs,B2400);	    /* Set output speed */
			 break;
			 }

			case   4800 : {
				cfsetispeed(&termattribs,B4800);	    /* Set input speed */
    				cfsetospeed(&termattribs,B4800);	    /* Set output speed */
			 break;
			 }

			case   9600 : {
				cfsetispeed(&termattribs,B9600);	    /* Set input speed */
    				cfsetospeed(&termattribs,B9600);	    /* Set output speed */
			 break;
			 }
			case   57600 : {
				cfsetispeed(&termattribs,B57600);	    /* Set input speed */
    				cfsetospeed(&termattribs,B57600);	    /* Set output speed */
			 break;
			 }

			default:   {

				cfsetispeed(&termattribs,B9600);	    /* Set input speed */
    				cfsetospeed(&termattribs,B9600);	    /* Set output speed */
			}
	}

	tcsetattr(fdSertnc,TCSANOW,&termattribs);  /* Set the serial port */

	mvprintw(6,0, "%s opened...", rigportname);
	refresh();

	mvprintw(13,0, "Input command: ");
	refresh();
	echo();
	getnstr(line,  12);
	noecho();
	strcat(line, eom);

/* send message */
	mvprintw (7,0, "sending message to trx: %s" , line );
	mvprintw(7,40, "Length = %d characters",strlen(line));
	refresh();

	write (fdSertnc, line, strlen(line));

	mvprintw (8,0, "receiving message from trx"); refresh();
	usleep(30000);


		if(fdSertnc > 0) {

			int j = 0;

//			i = read (fdSertnc, inputline, BUFFERSIZE-1);	### bug fix
			i = read (fdSertnc, inputline, sizeof(inputline));

			if (i > 0) {
				for (j=0; j<i; j++){
					mvprintw (10, j*10, "%#x", (char) inputline[j]);
					mvprintw(11, j, "%c", (char) inputline [j]);
					mvprintw(12, j * 10, "%d", (char) inputline[j] & 0xff);
					refresh();
				}
			}
			mvprintw(8,40, "Length = %d characters", i);
			if (inputline[0] == '@' && inputline [1] == 'A' && inputline[2] != 'F') {
				mvprintw(20, 0, "Frequency = %d Hz", ( (inputline[3] & 0xff) * 65536) + (( inputline[4] & 0xff) * 256) +  (inputline[5] & 0xff));
			}

			refresh();
			sleep(1);
		}


	mvprintw(23,0, "done"); refresh();
	i = getch();

/* close the tty */

	 if (fdSertnc > 0)	 close (fdSertnc);



return(0);
}