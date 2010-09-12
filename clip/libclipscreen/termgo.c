
/*
   $Log: termgo.c,v $
   Revision 1.1  2006/06/22 19:35:31  itk
   uri: init sf.net repository

   Revision 1.3  2000/12/02 12:46:57  clip
   scancodes
   paul

   Revision 1.2  1999/11/02 23:05:57  axl
   start of logging CVS

 */

#include <stdio.h>
#include <string.h>

#define MAXRETURNSIZE 64
	   /* max length of 'goto' string */
#define TPARMERR(c)     { strcpy (outp, (c)); return; }

static void tparm(char *outp, char *cp, int p1, int p2);

#include <termgo/tgoto.c>
#include <termgo/static_branchto.c>
#include <termgo/static_tparm.c>
