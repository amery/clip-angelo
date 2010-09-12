
/*
   $Log: termcap.c,v $
   Revision 1.1  2006/06/22 19:35:31  itk
   uri: init sf.net repository

   Revision 1.3  2000/12/02 12:46:57  clip
   scancodes
   paul

   Revision 1.2  1999/11/02 23:05:56  axl
   start of logging CVS

 */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

/* max length of termcap entry */
#define BUFSIZE  1024
/* max number of tc= indirections */
#define MAXHOP   32
#define NKEY	 100

static char SHARETERMCAP[] = "/usr/share/misc/termcap";

static char TERMCAP[] = "/etc/termcap";

static char MYTERMCAP[] = "/usr/lib/deco/termcap";

static char MY_LOCAL_TERMCAP[] = "/usr/local/lib/deco/termcap";

static char MYLCLTERMCAP[256];

static char *tname;		/* terminal name */

static char *tcapfile;		/* termcap file name */

static char *tbuf;		/* terminal entry buffer */

static char *envtermcap = 0;	/* global variable TERMCAP */

static hopcount;		/* detect infinite loops */

static char *tskip(char *);

static char *tdecode(char *, char **);

static int _tgetent(char *, char *, char *);

static int tnchktc();

static int tnamatch(char *);

char *tgetstr(char *str, char **area);

/* termcap statics ][ termcap init  */

#include <termcap/static_errexit.c>
#include <termcap/tgetent.c>
#include <termcap/static__tgetent.c>
#include <termcap/static_tnchktc.c>
#include <termcap/static_tnamatch.c>
#include <termcap/static_tskip.c>
#include <termcap/tgetstr.c>
#include <termcap/tgetflag.c>
#include <termcap/tgetnum.c>
#include <termcap/static_tdecode.c>
