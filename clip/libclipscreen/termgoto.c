
/*
   $Log: termgoto.c,v $
   Revision 1.1  2006/06/22 19:35:31  itk
   uri: init sf.net repository

   Revision 1.6  2004/05/21 11:22:19  clip
   rust: minor fix for 'configure -m'

   Revision 1.5  2000/12/02 12:46:57  clip
   scancodes
   paul

   Revision 1.4  2000/03/10 17:45:01  paul
   *** empty log message ***

   Revision 1.3  2000/03/10 15:56:11  paul
   xon/xoff gluk

   Revision 1.2  1999/11/02 23:05:58  axl
   start of logging CVS

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ci_clip.h>

#define MAXRETURNSIZE 64
	   /* max length of 'goto' string */
#define TPARMERR(c)     { strcpy (outp, (c)); return; }

static void tparm(char *outp, char *cp, int p1, int p2);

#include <termgoto/static_do_delay.c>
#include <termgoto/scr_tgoto.c>
#include <termgoto/scr_tputs.c>
#include <termgoto/static_branchto.c>
#include <termgoto/static_tparm.c>
