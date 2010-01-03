/*
   Copyright (C) 2003  JARA
   Authors  : Sergio Zayas
   License : (GPL) http://www.itk.ru/clipper/license.html
   mail : icaro.maneton@lycos.es

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include "ci_clip.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#ifndef OS_MINGW
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include "ci_hashcode.h"
#include "ci_error.ch"
#include "ci_fileio.ch"

#ifdef USE_UTMP
#  include <utmp.h>
#endif

#ifdef OS_CYGWIN
#define INET_ADDRSTRLEN  16
#define socklen_t int
#endif

/*****************************/
int
clip_TCPLISTEN(ClipMachine * ClipMachineMemory)
/* Create a socket and stand for connetions.
  Use : TcpListen( port[, nMaxConn] )
*/
{
   struct sockaddr_in sin;

   long      port = 0, timeout = 60000;

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror), i;

   char     *sport;

   int       sock = -1, ret = -1;

   int       backlog = 5;

   C_FILE   *cf = NULL;

   int       arg;

   *err = 0;

   if (_clip_parinfo(ClipMachineMemory, 0) < 1)
    {
       *err = EINVAL;
       goto err;
    }

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
      port = htons(_clip_parnl(ClipMachineMemory, 1));
   else if ((sport = _clip_parc(ClipMachineMemory, 1)) != NULL)
    {
       struct servent *sp;

       if ((sp = getservbyname((const char *) sport, "tcp")) != NULL)
	  port = sp->s_port;
       else
	{
	   for (i = 0; sport[i] >= '0' && sport[i] <= '9'; i++);
	   if (sport[i] == '\0')
	      port = htons(atol(sport));
	}
    }

   if (port == 0)
    {
       *err = EINVAL;
       goto err;
    }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      backlog = _clip_parnl(ClipMachineMemory, 2);

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      goto err;

   if ((arg = fcntl(sock, F_GETFL, 0)) == -1)
      goto err;
   fcntl(sock, F_SETFL, arg | O_NONBLOCK);

   memset((void *) &sin, 0, sizeof(sin));
   sin.sin_family = AF_INET;	/* PF_INET ?? */
   sin.sin_addr.s_addr = INADDR_ANY;
   sin.sin_port = port;

   if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1)
     /* Error, no bind */
    {

       *err = EFAULT;
       goto err;

    }

   if (listen(sock, backlog) == -1)
    {
       *err = EFAULT;
       goto err;
    }

   cf = (C_FILE *) calloc(1, sizeof(C_FILE));
   cf->fileno_of_C_FILE = sock;
   cf->file_of_C_FILE = NULL;
   cf->type_of_C_FILE = FT_SOCKET;
   cf->pid_of_C_FILE = -1;
   cf->timeout_of_C_FILE = timeout;
   cf->stat_of_C_FILE = 0;	/* see FS_* flags */
   ret = _clip_store_c_item(ClipMachineMemory, cf, _C_ITEM_TYPE_FILE, destroy_c_file);

 err:
   if (ret == -1)
    {
       if (*err != 0)
	  *err = errno;
       if (sock != -1)
	  close(sock);
    }
   _clip_retni(ClipMachineMemory, ret);

   return 0;
}

int
clip_TCPACCEPT(ClipMachine * ClipMachineMemory)
/* Return a new socket for pending connections.
	uso : tcpAccept( sock, timeout ) ->  nNewSocket  | -1 == error
*/
{
   int       ret = -1;

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int       sock = _clip_parni(ClipMachineMemory, 1);

   int       timeout = _clip_parni(ClipMachineMemory, 2);

   C_FILE   *cf = _clip_fetch_c_item(ClipMachineMemory, sock, _C_ITEM_TYPE_FILE);

   struct sockaddr_in sin;

   int       socknew;

   socklen_t sinlen = sizeof(sin);

   fd_set    readfd;

   struct timeval tv;

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
    {
       *err = EBADF;
    }
   else
    {

       if (_clip_parinfo(ClipMachineMemory, 2) != NUMERIC_type_of_ClipVarType)
	  timeout = cf->timeout_of_C_FILE;

       FD_ZERO(&readfd);
       FD_SET(cf->fileno_of_C_FILE, &readfd);
      //timeout = 0;
       tv.tv_sec = timeout / 1000;
       tv.tv_usec = (timeout % 1000) * 1000;

       if (select(cf->fileno_of_C_FILE + 1, &readfd, NULL, NULL, &tv) < 1)
	{
	   *err = errno;
	   _clip_retnl(ClipMachineMemory, ret);
	   return 0;
	}

       if ((socknew = accept(cf->fileno_of_C_FILE, (struct sockaddr *) &sin, &sinlen)) == -1)
	  *err = errno;
       else
	{
	  /* Accept correcto. */
	  // Se registra el socket en clip.
	   cf = (C_FILE *) calloc(1, sizeof(C_FILE));
	   cf->fileno_of_C_FILE = socknew;
	   cf->file_of_C_FILE = NULL;
	   cf->type_of_C_FILE = FT_SOCKET;
	   cf->pid_of_C_FILE = -1;
	   cf->timeout_of_C_FILE = timeout;	/* timeout; */
	   cf->stat_of_C_FILE = 0;	/* see FS_* flags */
	   ret = _clip_store_c_item(ClipMachineMemory, cf, _C_ITEM_TYPE_FILE, destroy_c_file);
	}

    }
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
