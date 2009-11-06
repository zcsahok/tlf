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
 	*      add call/band to dupe list
 	*
 	*--------------------------------------------------------------*/

#include "addcall.h"
 	
 	int addcall(void)
 	{

 	extern char hiscall[];
 	extern int callarray_nr;
 	extern char callarray[MAX_CALLS][20];
 	extern int call_country[MAX_CALLS];
 	extern char comment[];
 	extern char call_exchange[MAX_CALLS][12];
 	extern int cqww;
 	extern int bandinx;
 	extern int countries[MAX_DATALINES];
 	extern int zones[];
 	extern int countryscore[];
 	extern int addcty;
 	extern int zonescore[];
 	extern int addzone;
 	extern int countrynr;
 	extern int arrldx_usa;
 	extern int pacc_pa_flg;
	extern int universal;
	extern int country_mult;
 	extern int w_cty;
 	extern int ve_cty;
	extern int dx_arrlsections;
   extern int wazmult;
   extern int itumult;

static 	int found = 0;
static 	int  i, j, z = 0;
static 	char cbuffer[41] = "";
static 	char checkcall[20] = "";
static 	int add_ok;


 	    checkcall[0] = '\0';
 	    cbuffer[0] = '\0';

 	    strncat(checkcall, hiscall, 20);
 	
 	     found = searchcallarray(checkcall);

 	     if  (found == 0){

 		    	callarray_nr++;
 	    		i = callarray_nr;

	   	  	strcat(cbuffer,  hiscall);
 			strcat(cbuffer, "           ");
 			cbuffer[10] = '\0';
	   		strcpy(callarray[i], cbuffer);   	
          }
          else
          	i = found;
          	          	
          j  = getctydata(checkcall);
          call_country[i] = j;
          if  (strlen(comment)  >= 1){
              strcpy(call_exchange[i],comment);

              	if ((cqww ==1)||(wazmult ==1)||(itumult==1))  {
/*
			if (strlen(zone_fix) > 1) {
				z = zone_nr(zone_fix);
			} else
				z = zone_nr(zone_export);
*/
				z = zone_nr(comment);

		}
          }

 	     add_ok = 1;         /* look if certain calls are excluded */

 	     if ((arrldx_usa ==1) && ((countrynr == w_cty) || (countrynr == ve_cty)))
 	     	add_ok = 0;


		if((country_mult==1) && (universal ==1))
			add_ok=1;

 	     if ((dx_arrlsections ==1) && ((countrynr == w_cty) || (countrynr == ve_cty)))
 	     	add_ok = 0;


		if (pacc_pa_flg == 1)
 	     		add_ok = pacc_pa();

 	     if (add_ok == 1 )
		 {

 	       switch(bandinx)
 	       {
 	
 	       	 case BANDINDEX_160:{
 	       	 	
 	       	 	if (j != 0 && (countries[j] & BAND160) == 0){
 	       	 		countries[j] = (countries[j] | BAND160) ;
 	       	 	    countryscore[0]++;
 	       	 	    addcty = j;
 	       	 		}
 	       	 	if (z != 0 && (zones[z] & BAND160) == 0){
 	       	 		zones[z] = (zones[z] | BAND160);
 	       	 		zonescore[0]++;
 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	 	
 	       	}
 	       	case BANDINDEX_80:{

 	       	 	if (j != 0 && (countries[j] & BAND80) == 0){
 	       	 		countries[j] = (countries[j] | BAND80) ;
 	       	 		countryscore[1]++;
 	       	 		addcty = j;
 	       	 		}
                if (z != 0 && (zones[z] & BAND80) == 0){
 	       	 		zones[z] = (zones[z] | BAND80);
 	       	 		zonescore[1]++;
 	       	 		addzone = z;
 	       	 		}	       	 		
 	       	 	break;
 	       	}
 	       	case BANDINDEX_40:{
 	       	 	
 	       	 	if (j != 0 && (countries[j] & BAND40) == 0){
 	       	 		countries[j] = (countries[j] | BAND40) ;
 	       	 		countryscore[2]++;
 	       	 		addcty = j;
 	       	 		}
 	       	 	if (z != 0 && (zones[z] & BAND40) == 0){
 	       	 		zones[z] = (zones[z] | BAND40);
 	       	 		zonescore[2]++;
 	       	 		addzone = z;
 	       	 		}	       	 		
 	       	 	break;
 	       	}
 	       	case BANDINDEX_20:{
 	       	 	
 	       	 	if (j != 0 && (countries[j] & BAND20) == 0){
 	       	 		countries[j] = (countries[j] | BAND20) ;
 	       	 		countryscore[3]++;
 	       	 		addcty = j;
 	       	 		}
 	       	 	if (z != 0 && (zones[z] & BAND20) == 0){
 	       	 		zones[z] = (zones[z] | BAND20);
 	       	 		zonescore[3]++;
 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	}
 	       	case BANDINDEX_15:{
 	       	 	
 	       	 	if (j != 0 && (countries[j] & BAND15) == 0){
 	       	 		countries[j] = (countries[j] | BAND15) ;
 	       	 		countryscore[4]++;
 	       	 		addcty = j;
 	       	 		}
	       	 	if (z != 0 && (zones[z] & BAND15) == 0){
	       	 		zones[z] = (zones[z] | BAND15);
 	       	 		zonescore[4]++;
 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	}
 	       	case BANDINDEX_10:{
 	       	 	
 	       	 	if (j != 0 && (countries[j] & BAND10) == 0){
 	       	 		countries[j] = (countries[j] | BAND10) ;
 	       	 		countryscore[5]++;
 	       	 		addcty = j;
                     }
 	       	 	if (z != 0 && (zones[z] & BAND10) == 0){
 	       	 		zones[z] = (zones[z] | BAND10);
 	       	 		zonescore[5]++;
 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	}

 	     }

 	    }

 	  
	    		addmult();             /* for wysiwyg */
 	
 	   return(j);
}
/* -------------------------for network qso's-----------------------------------------*/
 	
int addcall2(void)
 	{

 	extern int callarray_nr;
 	extern char callarray[MAX_CALLS][20];
 	extern int call_country[MAX_CALLS];
 	extern int cqww;
 	extern int countries[MAX_DATALINES];
 	extern int zones[];
 	extern int countryscore[];
 	extern int zonescore[];
 	extern int pacc_pa_flg;
	extern int universal;
	extern int country_mult;
	extern char lan_logline[];
	extern int band_score[];
	extern int wpx;
	extern int nr_of_px;
	extern char prefixes_worked[MAX_CALLS][6];
   extern int wazmult;
   extern int itumult;
   extern char cqzone[];

int found = 0;
int  i, j, p,z = 0;
char cbuffer[41] = "";
char checkcall[20] = "";
int add_ok;
char lancopy[6];
char zonebuffer[4];

	char hiscall[20];
 	char comment[40];
 	int bandinx;
	int k;

 	    checkcall[0] = '\0';
 	    cbuffer[0] = '\0';


	strncpy (hiscall, lan_logline + 29, 20);

	for (k=0; k < strlen(hiscall); k++) {
		if (hiscall[k] == ' '){
			hiscall[k] = '\0';
			break;
		}
	}

	strncpy (comment, lan_logline + 54, 30);

	for (k=0; k < strlen(comment); k++) {
		if (comment[k] == ' '){
			comment[k] = '\0';
			break;
		}
	}



	strncat(checkcall, hiscall, 20);

 	     found = searchcallarray(checkcall);

 	     if  (found == 0){

 		    callarray_nr++;
 	    	i = callarray_nr;

	   	  	strcat(cbuffer,  hiscall);
 			strcat(cbuffer, "           ");
 			cbuffer[10] = '\0';
	   		strcpy(callarray[i], cbuffer);
          }
          else
          	i = found;

          
		  	strncpy (zonebuffer, cqzone, 3);	//hack: getctydata should not change zone!
		  j  = getctydata2(checkcall);
			strncpy (cqzone, zonebuffer, 3);	//idem....

          call_country[i] = j;
          if  (strlen(comment)  >= 1){
//              strcpy(call_exchange[i],comment);

              if ((cqww ==1)||(wazmult==1)||(itumult==1) )
              	z = zone_nr(comment);
          }

 	     add_ok = 1;         /* look if certain calls are excluded */

/* 	     if ((arrldx_usa ==1) && ((countrynr == w_cty) || (countrynr == ve_cty)))
 	     	add_ok = 0;
*/
         if((country_mult==1) && (universal ==1))
			add_ok=1;

		if (pacc_pa_flg == 1)
 	     		add_ok = pacc_pa();

 	     if (add_ok == 1) {

			bandinx = get_band (lan_logline);
 			band_score[bandinx]++;
			switch(bandinx)
 	        {

 	       	 case BANDINDEX_160:{

 	       	 	if (j != 0 && (countries[j] & BAND160) == 0)
				{
 	       	 		countries[j] = (countries[j] | BAND160) ;
 	       	 	    countryscore[0]++;
// 	       	 	    addcty = j;
				}
 	       	 	if (z != 0 && (zones[z] & BAND160) == 0)
				{
 	       	 		zones[z] = (zones[z] | BAND160);
 	       	 		zonescore[0]++;
// 	       	 		addzone = z;
				}
 	       	 	break;

 	       	}
 	       	case BANDINDEX_80:{

 	       	 	if (j != 0 && (countries[j] & BAND80) == 0){
 	       	 		countries[j] = (countries[j] | BAND80) ;
 	       	 		countryscore[1]++;
// 	       	 		addcty = j;
 	       	 		}
                if (z != 0 && (zones[z] & BAND80) == 0){
 	       	 		zones[z] = (zones[z] | BAND80);
 	       	 		zonescore[1]++;
// 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	}
 	       	case BANDINDEX_40:{

 	       	 	if (j != 0 && (countries[j] & BAND40) == 0){
 	       	 		countries[j] = (countries[j] | BAND40) ;
 	       	 		countryscore[2]++;
// 	       	 		addcty = j;
 	       	 		}
 	       	 	if (z != 0 && (zones[z] & BAND40) == 0){
 	       	 		zones[z] = (zones[z] | BAND40);
 	       	 		zonescore[2]++;
// 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	}
 	       	case BANDINDEX_20:{

 	       	 	if (j != 0 && (countries[j] & BAND20) == 0){
 	       	 		countries[j] = (countries[j] | BAND20) ;
 	       	 		countryscore[3]++;
// 	       	 		addcty = j;
 	       	 		}
 	       	 	if (z != 0 && (zones[z] & BAND20) == 0){
 	       	 		zones[z] = (zones[z] | BAND20);
 	       	 		zonescore[3]++;
// 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	}
 	       	case BANDINDEX_15:{

 	       	 	if (j != 0 && (countries[j] & BAND15) == 0){
 	       	 		countries[j] = (countries[j] | BAND15) ;
 	       	 		countryscore[4]++;
// 	       	 		addcty = j;
 	       	 		}
	       	 	if (z != 0 && (zones[z] & BAND15) == 0){
	       	 		zones[z] = (zones[z] | BAND15);
 	       	 		zonescore[4]++;
// 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	}
 	       	case BANDINDEX_10:{

 	       	 	if (j != 0 && (countries[j] & BAND10) == 0){
 	       	 		countries[j] = (countries[j] | BAND10) ;
 	       	 		countryscore[5]++;
// 	       	 		addcty = j;
                     }
 	       	 	if (z != 0 && (zones[z] & BAND10) == 0){
 	       	 		zones[z] = (zones[z] | BAND10);
 	       	 		zonescore[5]++;
// 	       	 		addzone = z;
 	       	 		}
 	       	 	break;
 	       	}

 	     }
 	    }
		if (wpx ==1){

			if (lan_logline[68] != ' ') {

				strcpy(lancopy,"     ");
				strncpy(lancopy, lan_logline+68, 4);

				for (p=0; p<=5; p++) {

					if (lancopy[p]==' ') {
						lancopy[p] = '\0';
						break;
					}
				}


				for (p=0; p <= nr_of_px; p++){


        	 				if (strcmp(lancopy, prefixes_worked[p]) == 0){
        	 	    				found = 1;
        	 					break;

        	 				}
        				}
        				if (found != 1){
        					strcpy(prefixes_worked[nr_of_px], lancopy);
        		 			nr_of_px++;
				}

			}
		}

 	    addmult2();             /* for wysiwyg from LAN*/

 	   return(j);
}

int get_band (char *logline) {

int j = 0;

	switch (atoi(logline)) {

		case 160 :
			j = BANDINDEX_160;
			break;

		case 80 :
			j = BANDINDEX_80;
			break;

		case 40 :
			j = BANDINDEX_40;
			break;

		case 20 :
			j = BANDINDEX_20;
			break;

		case 15 :
			j = BANDINDEX_15;
			break;

		case 10 :
			j = BANDINDEX_10;
			break;

	}

return(j);
}