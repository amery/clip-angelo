
/*
   $Log: kbd.c,v $
   Revision 1.1  2006/06/22 19:35:31  itk
   uri: init sf.net repository

   Revision 1.6  2004/07/08 07:21:05  clip
   uri: small fix for kbdstat()

   Revision 1.5  2001/10/29 06:51:21  clip
   fix text fopen modes
   paul

   Revision 1.4  2001/05/30 10:19:43  clip
   VT switch in scan mode on console
   paul

   Revision 1.3  2000/12/02 13:47:40  clip
   CLIPROOT now may be envar
   paul

   Revision 1.2  2000/12/02 12:46:57  clip
   scancodes
   paul

   Revision 1.1  2000/11/30 14:57:15  clip
   scancode support
   paul

 */

#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

#include <ci_scankey.h>

char *CLIPROOT = ".";

static struct termios ts, ts0;

int scr_scan_mode = 0;

#include <kbd/static_exit_f.c>
#include <kbd/static_sig_f.c>
#include <kbd/main.c>
