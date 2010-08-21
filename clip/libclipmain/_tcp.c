/*
   Copyright (C) 2002  ITK
   Authors  : Uri Hnykin <uri@itk.ru>, Przemyslaw <druzus@acn.waw.pl>
   License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <string.h>
#include <ci_clip.h>

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

#include <ci_clipcfg.h>
#include <ci_hashcode.h>
#include <ci_error.ch>
#include <ci_fileio.ch>

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

#include <_tcp/static_inet_aton.c>

#endif

#include <_tcp/static_tcp_host_addr.c>
#include <_tcp/static_tcp_host_name.c>
#include <_tcp/static_tcp_term_addr.c>
#include <_tcp/clip_IPADDRTRANS.c>
#include <_tcp/clip_IPADDR2BIN.c>
#include <_tcp/clip_BIN2IPADDR.c>
#include <_tcp/clip_GETHOSTBYNAME.c>
#include <_tcp/clip_GETHOSTBYADDR.c>
#include <_tcp/clip_GETTERMADDR.c>
#include <_tcp/clip_GETHOSTNAME.c>
#include <_tcp/clip_GETDOMAINNAME.c>
#include <_tcp/clip_TCPCONNECT.c>
#include <_tcp/clip_TCPREAD.c>
#include <_tcp/clip_TCPWRITE.c>
#include <_tcp/static__clip_sockclose.c>
#include <_tcp/clip_TCPCLOSE.c>
#include <_tcp/clip_UDPSOCKET.c>
#include <_tcp/clip_UDPCLOSE.c>
#include <_tcp/clip_UDPBROADCAST.c>
#include <_tcp/clip_UDPBIND.c>
#include <_tcp/clip_UDPSENDTO.c>
#include <_tcp/clip_UDPRECVFROM.c>
#include <_tcp/clip_TCPGETPEERNAME.c>
