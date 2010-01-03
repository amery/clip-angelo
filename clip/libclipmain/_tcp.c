/*
   Copyright (C) 2002  ITK
   Authors  : Uri Hnykin <uri@itk.ru>, Przemyslaw <druzus@acn.waw.pl>
   License : (GPL) http://www.itk.ru/clipper/license.html

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

#include "ci_clipcfg.h"
#include "ci_hashcode.h"
#include "ci_error.ch"
#include "ci_fileio.ch"

#ifdef USE_UTMP
#  include <utmp.h>
#endif

#ifdef OS_CYGWIN
#define INET_ADDRSTRLEN  16
#endif

#ifndef INADDR_NONE
#define INADDR_NONE 0
#endif

#ifdef OS_SUNOS
static int
inet_aton(const char *host, struct in_addr *ina)
{
   in_addr_t a;

   a = inet_addr(host);
   if (((int) a) == -1)
      return -1;

   ina->s_addr = a;

   return 0;
}

#endif

/* get host inetrnet address and store it in ina */
static int
tcp_host_addr(const char *host, struct in_addr *ina)
{
   int       ret = -1;

   if (host != NULL && *host != '\0')
    {
      /* gethostbyname in WinSock doesn't work with address in
         standard numbers-and-dots notation so check for n&d first */
       if (inet_aton(host, ina) != 0)
	  ret = 0;
       else
	{
	   struct hostent *hp = gethostbyname(host);

	   if (hp && hp->h_addrtype == AF_INET && hp->h_addr_list != NULL && hp->h_addr_list[0] != NULL)
	    {
	       memcpy(ina, hp->h_addr_list[0], hp->h_length);
	       ret = 0;
	    }
	}
    }

   if (ret == -1)
      ina->s_addr = INADDR_NONE;
   else if (ina->s_addr == INADDR_NONE)
      ret = -1;

   return ret;
}

/* get name of host pointed by host */
static const char *
tcp_host_name(const char *host)
{
   struct in_addr ina;

   const char *ret = "";

   if (host != NULL && *host != '\0')
    {
       struct hostent *hp;

       if (inet_aton(host, &ina) != 0)
	  hp = gethostbyaddr((const char *) &ina, sizeof(struct in_addr), AF_INET);
       else
	  hp = gethostbyname(host);

       if (hp && hp->h_addrtype == AF_INET)
	  ret = hp->h_name;
    }
   return ret;
}

/* try to detected remote terminal IP address */
static int
tcp_term_addr(struct in_addr *ina)
{
   char     *env_val, *ptr;

   int       ret = -1;

  /* REMOTEHOST is set by telnetd */
   if ((env_val = getenv("REMOTEHOST")) != NULL && tcp_host_addr(env_val, ina) == 0)
      ret = 0;

  /* SSH_CLIENT is set by sshd but some of
     implementation doesn't 'export' it */
   if (ret == -1 && (env_val = getenv("SSH_CLIENT")) != NULL)
    {
       if ((ptr = strchr(env_val, ' ')) != NULL)
	  *ptr = '\0';
       if (tcp_host_addr(env_val, ina) == 0)
	  ret = 0;
    }

#ifdef USE_UTMP
  /* If we cannot find a proper address in environment we have
     to check UTMP entries for our current terminal */
   if (ret == -1)
    {
       struct utmp entry, *ut;

       if ((ptr = ttyname(0)) != NULL)
	{
	   if (strncmp(ptr, "/dev/", 5) == 0)
	      ptr += 5;
	   strcpy(entry.ut_line, ptr);
	   setutent();
	   if ((ut = getutline(&entry)) != NULL &&
	       ut->ut_addr != 0 && ut->ut_addr != INADDR_NONE &&
	       (ptr = inet_ntoa(*(struct in_addr *) &ut->ut_addr)) != NULL && tcp_host_addr(ptr, ina) == 0)
	      ret = 0;
	   endutent();
	}
    }
#endif
   if (ret == -1)
      ina->s_addr = INADDR_NONE;
   return ret;
}

/*****************************/
int
clip_IPADDRTRANS(ClipMachine * ClipMachineMemory)
{
   char     *addr = _clip_parc(ClipMachineMemory, 1), *cf = _clip_parc(ClipMachineMemory, 2);

   char     *ret = "", buf[100], tmpl[30];

   unsigned long b1, b2, b3, b4, nf;


   if (addr != NULL)
    {

       nf = sscanf(addr, "%lu%*[.]%lu%*[.]%lu%*[.]%lu", &b1, &b2, &b3, &b4);
       if (nf == 4)
	{
	   if (cf == NULL)
	      cf = "";
	   snprintf(tmpl, sizeof(tmpl), "%%%sd.%%%sd.%%%sd.%%%sd", cf, cf, cf, cf);
	   snprintf(buf, sizeof(buf), tmpl, b1, b2, b3, b4);
	   ret = buf;
	}
    }

   _clip_retc(ClipMachineMemory, ret);
   return 0;
}

/*****************************/
int
clip_IPADDR2BIN(ClipMachine * ClipMachineMemory)
{
   char     *addr = _clip_parc(ClipMachineMemory, 1);

   struct in_addr ina;


   if (tcp_host_addr(addr, &ina) == 0)
      _clip_retcn(ClipMachineMemory, (char *) &ina, sizeof(struct in_addr));
   else
      _clip_retc(ClipMachineMemory, "");

   return 0;
}

/*****************************/
int
clip_BIN2IPADDR(ClipMachine * ClipMachineMemory)
{
   int       len;

   char     *addr = _clip_parcl(ClipMachineMemory, 1, &len);


   if (addr == NULL || len != sizeof(struct in_addr))
      _clip_retc(ClipMachineMemory, "");
   else
      _clip_retc(ClipMachineMemory, inet_ntoa(*(struct in_addr *) addr));

   return 0;
}

/*****************************/
int
clip_GETHOSTBYNAME(ClipMachine * ClipMachineMemory)
{
   char     *addr = _clip_parc(ClipMachineMemory, 1);

   struct in_addr ina;


   if (tcp_host_addr(addr, &ina) == 0)
      _clip_retc(ClipMachineMemory, inet_ntoa(ina));
   else
      _clip_retc(ClipMachineMemory, "");

   return 0;
}

/*****************************/
int
clip_GETHOSTBYADDR(ClipMachine * ClipMachineMemory)
{
   char     *addr = _clip_parc(ClipMachineMemory, 1);


   if (addr == NULL || *addr == 0)
      _clip_retc(ClipMachineMemory, "");
   else
      _clip_retc(ClipMachineMemory, (char *) tcp_host_name(addr));

   return 0;
}

/*****************************/
int
clip_GETTERMADDR(ClipMachine * ClipMachineMemory)
{
   struct in_addr ina;


   if (tcp_term_addr(&ina) == 0)
      _clip_retc(ClipMachineMemory, inet_ntoa(ina));
   else
      _clip_retc(ClipMachineMemory, "");

   return 0;
}

/*****************************/
int
clip_GETHOSTNAME(ClipMachine * ClipMachineMemory)
{
  /* return host name of process */
   char      buf[256];

   if (gethostname(buf, 255) != 0)
      _clip_retc(ClipMachineMemory, "");
   else
      _clip_retc(ClipMachineMemory, buf);
   return 0;
}

/*****************************/
int
clip_GETDOMAINNAME(ClipMachine * ClipMachineMemory)
{
  /* return host domain name of process */
   char      buf[256];

#ifdef OS_MINGW
   DWORD     s;

   GetComputerName(buf, &s);
#else
   if (getdomainname(buf, 255) != 0)
      _clip_retc(ClipMachineMemory, "");
   else
#endif
      _clip_retc(ClipMachineMemory, buf);
   return 0;
}

/*****************************/
int
clip_TCPCONNECT(ClipMachine * ClipMachineMemory)
{
   C_FILE   *cf = NULL;

   struct sockaddr_in sin;

   long      port = 0, timeout = 60000;	/* maybe we should add _set_ for default timeout */

   int       arg = 0, sock = -1, ret = -1, i;

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   struct timeval tv;

   char     *addr = _clip_parc(ClipMachineMemory, 1), *sport;

   *err = 0;

   if (_clip_parinfo(ClipMachineMemory, 0) < 2 || _clip_parinfo(ClipMachineMemory, 1) != CHARACTER_type_of_ClipVarType)
    {
       *err = EINVAL;
       goto err;
    }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      port = htons(_clip_parnl(ClipMachineMemory, 2));
   else if ((sport = _clip_parc(ClipMachineMemory, 2)) != NULL)
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

   if (_clip_parinfo(ClipMachineMemory, 3) == NUMERIC_type_of_ClipVarType)
      timeout = _clip_parnl(ClipMachineMemory, 3);

   tv.tv_sec = timeout / 1000;
   tv.tv_usec = (timeout % 1000) * 1000;

   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      goto err;

/*
#if !defined(linux) && !defined(SOLARIS_26_X86)
	setsockopt( sock, SOL_SOCKET, SO_SNDTIMEO, (void *)&tv,  sizeof(tv) );
	setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO, (void *)&tv,  sizeof(tv) );
#endif
*/
   if ((arg = fcntl(sock, F_GETFL, 0)) == -1)
      goto err;
   fcntl(sock, F_SETFL, arg | O_NONBLOCK);

   sin.sin_family = AF_INET;
   sin.sin_port = port;
   tcp_host_addr(addr, &sin.sin_addr);

   if (sin.sin_addr.s_addr == INADDR_NONE)
    {
       *err = EFAULT;
       goto err;
    }

   if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1)
    {
       fd_set    set;

#ifndef OS_MINGW
       if (errno != EINPROGRESS)
	  goto err;
#endif

       FD_ZERO(&set);
       FD_SET(sock, &set);

       do
	  i = _clip_select(sock + 1, NULL, &set, NULL, &tv);
       while (i == -1 && errno == EINTR);

       if (i == -1)
	  goto err;
       else if (i == 0)
	{
#ifdef OS_MINGW
	   *err = EAGAIN;
#else
	   *err = ETIMEDOUT;
#endif
	}

       arg = 0;
       i = sizeof(arg);
       if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (void *) &arg, (socklen_t *) (&i)) == -1)
	  goto err;
       if (arg != 0)
	{
	   *err = arg;
	   goto err;
	}
    }

#ifndef OS_MINGW
   if ((arg = fcntl(sock, F_GETFL, 0)) == -1)
      goto err;
   fcntl(sock, F_SETFL, arg | O_NONBLOCK);
#endif

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
clip_TCPREAD(ClipMachine * ClipMachineMemory)
{
   int       lenbuf = 0, ret = -1;

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int       fd = _clip_parni(ClipMachineMemory, 1);

   char     *buf = _clip_parcl(ClipMachineMemory, 2, &lenbuf);

   long      len = _clip_parnl(ClipMachineMemory, 3);

   int       timeout = _clip_parni(ClipMachineMemory, 4), oldtout;

   C_FILE   *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   _clip_retnl(ClipMachineMemory, -1);

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
    {
       *err = EBADF;
    }
   else
    {
       if (lenbuf < len)
	  len = lenbuf;

       oldtout = cf->timeout_of_C_FILE;
       if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType)
	  cf->timeout_of_C_FILE = timeout;

       if (len > 0)
	{
	   if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
	      ret = _clip_read(cf, buf, len);
	   else
	      ret = fread(buf, 1, len, cf->file_of_C_FILE);
	}
       else
	  ret = 0;

       cf->timeout_of_C_FILE = oldtout;

       *err = ret < 0 ? errno : 0;
    }
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}

int
clip_TCPWRITE(ClipMachine * ClipMachineMemory)
{
   int       lenbuf = 0, ret = -1;

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int       fd = _clip_parni(ClipMachineMemory, 1);

   char     *buf = _clip_parcl(ClipMachineMemory, 2, &lenbuf);

   long      len = _clip_parnl(ClipMachineMemory, 3);

   int       timeout = _clip_parni(ClipMachineMemory, 4), oldtout;

   C_FILE   *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);


   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
    {
       if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType || len > lenbuf)
	  len = lenbuf;

       oldtout = cf->timeout_of_C_FILE;
       if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType)
	  cf->timeout_of_C_FILE = timeout;

       if (len > 0)
	{
	   if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
	      ret = _clip_write(cf, buf, len);
	   else
	      ret = fwrite(buf, 1, len, cf->file_of_C_FILE);
	}
       else
	  ret = 0;

       cf->timeout_of_C_FILE = oldtout;

       *err = ret < 0 ? errno : 0;
    }
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}

static int
_clip_sockclose(ClipMachine * ClipMachineMemory)
{
   int       fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE   *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   int       ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   fd_set    rfds;

   struct timeval tv;

   int       arg;


   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
    {
       *err = EBADF;
    }
   arg = fcntl(cf->fileno_of_C_FILE, F_GETFL, 0);
   if (!(arg & O_NONBLOCK))
    {
       FD_ZERO(&rfds);
       FD_SET(cf->fileno_of_C_FILE, &rfds);
       tv.tv_sec = 1;		//timeout / 1000;
       tv.tv_usec = 100;	//(timeout % 1000) * 1000;
       select(cf->fileno_of_C_FILE + 1, &rfds, NULL, NULL, &tv);
    }
   if (_clip_destroy_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE))
    {
       *err = ret = 0;
    }
   else
    {
       *err = errno;
    }

  /* _clip_retni(ClipMachineMemory, ret); TODO? What type of value should return */
   _clip_retl(ClipMachineMemory, (ret == 0));
   return 0;
}

int
clip_TCPCLOSE(ClipMachine * ClipMachineMemory)
{
   return _clip_sockclose(ClipMachineMemory);
}

int
clip_UDPSOCKET(ClipMachine * ClipMachineMemory)
{
   C_FILE   *cf = NULL;

   int       arg = 0, sock = -1, ret = -1;

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   *err = 0;
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
      goto err;

#ifndef OS_MINGW
   if ((arg = fcntl(sock, F_GETFL, 0)) == -1)
      goto err;
   fcntl(sock, F_SETFL, arg | O_NONBLOCK);
#endif

   cf = (C_FILE *) calloc(1, sizeof(C_FILE));
   cf->fileno_of_C_FILE = sock;
   cf->file_of_C_FILE = NULL;
   cf->type_of_C_FILE = FT_SOCKET;
   cf->pid_of_C_FILE = -1;
   cf->timeout_of_C_FILE = 60000;
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
clip_UDPCLOSE(ClipMachine * ClipMachineMemory)
{
   return _clip_sockclose(ClipMachineMemory);
}

int
clip_UDPBROADCAST(ClipMachine * ClipMachineMemory)
{
   int       fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE   *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror), ret = -1, arg;


   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
    {
       arg = _clip_parl(ClipMachineMemory, 2);
       ret = setsockopt(cf->fileno_of_C_FILE, SOL_SOCKET, SO_BROADCAST, (void *) &arg, sizeof(arg));
       *err = ret == -1 ? errno : 0;
    }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_UDPBIND(ClipMachine * ClipMachineMemory)
{
   int       fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE   *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   char     *ipaddr = _clip_parc(ClipMachineMemory, 2);

   int       port = _clip_parni(ClipMachineMemory, 3);

   int       ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   struct sockaddr_in sin;


   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
    {
       memset((void *) &sin, 0, sizeof(sin));
       sin.sin_family = PF_INET;
       sin.sin_addr.s_addr = inet_addr(ipaddr);
       sin.sin_port = htons(port);
       ret = bind(cf->fileno_of_C_FILE, (struct sockaddr *) &sin, sizeof(sin));
       *err = ret == -1 ? errno : 0;
    }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}

int
clip_UDPSENDTO(ClipMachine * ClipMachineMemory)
// UDPSENDTO( nCF, cIPaddr, nPort, cMsg, [nLen], [nTimeout] )
{
   int       fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE   *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   char     *ipaddr = _clip_parc(ClipMachineMemory, 2);

   int       port = _clip_parni(ClipMachineMemory, 3), lenbuf;

   char     *msg = _clip_parcl(ClipMachineMemory, 4, &lenbuf);

   long      len = _clip_parnl(ClipMachineMemory, 5);

   int       timeout = _clip_parni(ClipMachineMemory, 6);

   int       ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   struct sockaddr_in sin;

   struct timeval tv;

   fd_set    set;


   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
    {
       if (_clip_parinfo(ClipMachineMemory, 5) != NUMERIC_type_of_ClipVarType)
	  len = lenbuf;
       if (len > lenbuf)
	  len = lenbuf;

       if (_clip_parinfo(ClipMachineMemory, 6) != NUMERIC_type_of_ClipVarType)
	  timeout = cf->timeout_of_C_FILE;

       if (timeout >= 0)
	{
	   FD_ZERO(&set);
	   FD_SET(cf->fileno_of_C_FILE, &set);
	   tv.tv_sec = timeout / 1000;
	   tv.tv_usec = (timeout % 1000) * 1000;
	}

       memset((void *) &sin, 0, sizeof(sin));
       sin.sin_family = PF_INET;
       sin.sin_addr.s_addr = inet_addr(ipaddr);
       sin.sin_port = htons(port);

       if (timeout < 0 || select(cf->fileno_of_C_FILE + 1, NULL, &set, NULL, &tv) > 0)
	{
	   ret = sendto(cf->fileno_of_C_FILE, msg, len, 0, (struct sockaddr *) &sin, sizeof(sin));
	   *err = ret == -1 ? errno : 0;
	}
       else
#ifdef OS_MINGW
	  *err = EAGAIN;
#else
	  *err = ETIMEDOUT;
#endif

    }

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}

int
clip_UDPRECVFROM(ClipMachine * ClipMachineMemory)
// UDPRECVFROM( nCF, @cIPadr, @nPort, @cMsg, [nLen], [nTimeout] )
{
   int       fd = _clip_parni(ClipMachineMemory, 1), lenbuf, sln;

   C_FILE   *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   char     *msg = _clip_parcl(ClipMachineMemory, 4, &lenbuf);

   long      len = _clip_parnl(ClipMachineMemory, 5);

   int       timeout = _clip_parni(ClipMachineMemory, 6);

   int       ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   char      ipaddr[INET_ADDRSTRLEN];

   struct sockaddr_in sin;

   struct timeval tv;

   fd_set    set;


   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
    {
       if (_clip_parinfo(ClipMachineMemory, 5) != NUMERIC_type_of_ClipVarType || len > lenbuf)
	  len = lenbuf;

       if (_clip_parinfo(ClipMachineMemory, 6) != NUMERIC_type_of_ClipVarType)
	  timeout = cf->timeout_of_C_FILE;

       if (timeout >= 0)
	{
	   FD_ZERO(&set);
	   FD_SET(cf->fileno_of_C_FILE, &set);
	   tv.tv_sec = timeout / 1000;
	   tv.tv_usec = (timeout % 1000) * 1000;
	}

       memset((void *) &sin, 0, sizeof(sin));
       sin.sin_family = PF_INET;
       sln = sizeof(sin);
       if (timeout < 0 || select(cf->fileno_of_C_FILE + 1, &set, NULL, NULL, &tv) > 0)
	  ret = recvfrom(cf->fileno_of_C_FILE, msg, len, 0, (struct sockaddr *) &sin, (socklen_t *) (&sln));
       else
#ifdef OS_MINGW
	  *err = EAGAIN;
#else
	  *err = ETIMEDOUT;
#endif

       if (ret >= 0)
	{
#ifdef _WIN32
	   _clip_storc(ClipMachineMemory, inet_ntoa(*(struct in_addr *) ipaddr), 2, 0);
	   _clip_storni(ClipMachineMemory, ntohs(sin.sin_port), 3, 0);
#else
	   if (inet_ntop(PF_INET, &sin.sin_addr.s_addr, ipaddr, INET_ADDRSTRLEN) == NULL)
	      ret = -1;
	   else
	    {
	       _clip_storc(ClipMachineMemory, ipaddr, 2, 0);
	       _clip_storni(ClipMachineMemory, ntohs(sin.sin_port), 3, 0);
	    }
#endif
	}

       *err = ret == -1 ? errno : 0;
    }

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}

int
clip_TCPGETPEERNAME(ClipMachine * ClipMachineMemory)
/* Get the address of the peer to which a socket is connected.
   using : tcpGetPeerName( sock, @cIPadr ) ->  0 - OK  | -1 == error
*/
{
   int       ret = -1, sln;

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int       fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE   *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   char      ipaddr[INET_ADDRSTRLEN];

   struct sockaddr_in sin;


   _clip_retnl(ClipMachineMemory, -1);

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
    {
       *err = EBADF;
    }
   else
    {
       sln = sizeof(sin);
       ret = getpeername(cf->fileno_of_C_FILE, (struct sockaddr *) &sin, (socklen_t *) (&sln));

       *err = ret < 0 ? errno : 0;

       if (ret >= 0)
	{
#ifdef _WIN32
	   _clip_storc(ClipMachineMemory, inet_ntoa(*(struct in_addr *) ipaddr), 2, 0);
#else
	   if (inet_ntop(PF_INET, &sin.sin_addr.s_addr, ipaddr, INET_ADDRSTRLEN) == NULL)
	      ret = -1;
	   else
	      _clip_storc(ClipMachineMemory, ipaddr, 2, 0);
#endif
	   _clip_storni(ClipMachineMemory, ntohs(sin.sin_port), 3, 0);
	}
       else
	  _clip_storni(ClipMachineMemory, -1, 3, 0);

    }

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
