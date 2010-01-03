/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include "ci_clip.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef OS_MINGW
#include <sys/wait.h>
#endif
#include <sys/time.h>

#include "ci_error.ch"

#ifndef _WIN32
static char in_path[256] = { 0 };
static char out_path[256] = { 0 };

static FILE *fin = 0;

static FILE *fout = 0;

static pid_t pid = 0, tpid = 0;

static int cleanup_installed = 0;

static void
cleanup(void)
{
   if (fin)
    {
       fclose(fin);
       fin = 0;
    }
   if (fout)
    {
       fclose(fout);
       fout = 0;
    }
   if (in_path[0])
    {
       remove(in_path);
       in_path[0] = 0;
    }
   if (out_path[0])
    {
       remove(out_path);
       out_path[0] = 0;
    }
   if (tpid != 0)
    {
       int       status;

       kill(tpid, SIGTERM);
       waitpid(tpid, &status, 0);
       tpid = 0;
    }
}

static void
dbg_signal(int sig)
{
}
#endif

/*
	dbgconnect(cProgramNname|nPid) -> lResult
*/
int
clip_DBGCONNECT(ClipMachine * ClipMachineMemory)
{
#ifdef _WIN32
   return EG_ARG;
#else
   ClipVar  *vp = _clip_par(ClipMachineMemory, 1);

   pid_t     dpid = 0;

   if (!vp)
      return EG_ARG;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
      dpid = _clip_double(vp);
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
    {
       char      path[256];

       FILE     *file;

       char     *s = _clip_parc(ClipMachineMemory, 1);

       if (!s || !*s)
	  return EG_ARG;

       snprintf(path, sizeof(path), "pidof -s %s", s);
       file = popen(path, "r");
       if (file)
	{
	   unsigned long ul;

	   if (fscanf(file, "%lu", &ul) == 1)
	      dpid = ul;
	   pclose(file);
	}
    }

   if (!dpid)
      return EG_ARG;

   pid = dpid;

   if (!cleanup_installed)
    {
       cleanup_installed = 1;
       signal(SIGPIPE, dbg_signal);
       atexit(cleanup);
    }

   snprintf(in_path, sizeof(in_path), "/tmp/clip_dbg.%lu.in", (unsigned long) pid);
   remove(in_path);
   if (mkfifo(in_path, 0600))
    {
       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			 ClipMachineMemory->fp->line_of_ClipFrame,
			 "DBGCONNECT: cannot create FIFO '%s': %s\n", in_path, strerror(errno));
       cleanup();
       return -1;
    }

   _clip_logg(1, "fifo %s created successfully", in_path);

   snprintf(out_path, sizeof(out_path), "/tmp/clip_dbg.%lu.out", (unsigned long) pid);
   remove(out_path);
   if (mkfifo(out_path, 0600))
    {
       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			 ClipMachineMemory->fp->line_of_ClipFrame,
			 "DBGCONNECT: cannot create FIFO '%s': %s\n", out_path, strerror(errno));
       cleanup();
       return -1;
    }
   _clip_logg(1, "fifo %s created successfully", out_path);

   if (kill(pid, SIGUSR1))
    {
       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			 ClipMachineMemory->fp->line_of_ClipFrame,
			 "DBGCONNECT: cannot send signal SIGUSR1 to pid %lu: %s\n", (unsigned long) pid, strerror(errno));
       cleanup();
       return -1;
    }
   _clip_logg(1, "signal SIGUSR1 sent to pid %lu", (unsigned long) pid);

   fin = fopen(in_path, "wt");
   if (!fin)
    {
       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			 ClipMachineMemory->fp->line_of_ClipFrame,
			 "DBGCONNECT: cannot open FIFO '%s': %s\n", in_path, strerror(errno));
       cleanup();
       return -1;
    }
   setvbuf(fin, 0, _IOLBF, 0);
   fwrite("\n", 1, 1, fin);
   _clip_logg(1, "fifo %s opened for writing", in_path);

   fout = fopen(out_path, "rt");
   if (!fout)
    {
       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			 ClipMachineMemory->fp->line_of_ClipFrame,
			 "DBGCONNECT: cannot open FIFO '%s': %s\n", out_path, strerror(errno));
       cleanup();
       return -1;
    }
  /*setvbuf(fout, 0, _IOLBF, 0); */

   _clip_logg(1, "fifo %s opened for reading", out_path);
   _clip_retl(ClipMachineMemory, 1);

   for (;;)
    {
       char      buf[4096];

       if (!fgets(buf, 4096, fout))
	{
	   _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			     ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: cannot read data");
	   cleanup();
	   return -1;
	}
       if (!strcmp(buf, ".\n"))
	  break;
    }

   return 0;
#endif
}

int
clip_DBGCLOSE(ClipMachine * ClipMachineMemory)
{
   return 0;
}

/*
	dbgcommand(cCommand) -> cResponce
*/
int
clip_DBGCOMMAND(ClipMachine * ClipMachineMemory)
{
#ifdef _WIN32
   return EG_ARG;
#else
   char     *str = _clip_parc(ClipMachineMemory, 1);

   char     *buf, *obuf = 0;

   if (!str)
      return EG_ARG;

   if (!fin)
    {
       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			 ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: no connection");
       return -1;
    }

   buf = (char *) malloc(4096);
   memset(buf, 0, 4096);

#if 0
  /* read all possible input */
   for (;;)
    {
       fd_set    rfds;

       int       r, fno = fileno(fout);

       struct timeval tv;

       FD_ZERO(&rfds);
       FD_SET(fno, &rfds);
       tv.tv_sec = 0;
       tv.tv_usec = 0;

       r = select(fno + 1, &rfds, 0, 0, &tv);

       if (r <= 0)
	  break;

       for (;;)
	{
	   if (!fgets(buf, 4096, fout))
	    {
	       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
				 ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: cannot read data");
	       cleanup();
	       free(buf);
	       return -1;
	    }
	  /*printf("got: %s", buf); */
	   if (!strcmp(buf, ".\n"))
	    {
	       fflush(fout);
	       break;
	    }
	}
    }
#endif

   fprintf(fin, "%s\n", str);
   fflush(fin);

   if (kill(pid, SIGUSR1))
    {
       _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			 ClipMachineMemory->fp->line_of_ClipFrame,
			 "DBGCOMMAND: cannot send signal to PID %lu\n", (unsigned long) pid);
       cleanup();
       free(buf);
       return -1;
    }

   usleep(1);

  /* read input up to '.' line */
   obuf = (char *) malloc(1);
   *obuf = 0;

   for (;;)
    {
       int       l, ol;

       if (!fgets(buf, 4096, fout))
	{
	   _clip_trap_printf(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame,
			     ClipMachineMemory->fp->line_of_ClipFrame, "DBGCOMMAND: cannot read data");
	   cleanup();
	   free(buf);
	   free(obuf);
	   return -1;
	}
      /*printf("got: %s", buf); */
       if (!strcmp(buf, ".\n"))
	  break;
       l = strlen(buf);
       ol = strlen(obuf);
       obuf = (char *) realloc(obuf, ol + l + 1);
       memcpy(obuf + ol, buf, l);
       obuf[ol + l] = 0;
    }

   _clip_retcn_m(ClipMachineMemory, obuf, strlen(obuf));
   free(buf);
   return 0;
#endif
}
