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

#include <ci_hashcode.h>
#include <ci_error.ch>
#include <ci_fileio.ch>

#ifdef USE_UTMP
#  include <utmp.h>
#endif

#ifdef OS_CYGWIN
#define INET_ADDRSTRLEN  16
#define socklen_t int
#endif

#include <_tcpserv/clip_TCPLISTEN.c>
#include <_tcpserv/clip_TCPACCEPT.c>
