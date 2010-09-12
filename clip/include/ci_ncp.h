/*
	Copyright (C) 2002  ITK
	Author  : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
*/
#ifndef CN_NCP_H
#define CN_NCP_H

#include "ci_clipcfg.h"

#define USE_NCPFS_LOCK 1

int ncp_is_ncpfs(int fd);

int ncp_is_ncpfs_filename(char *filename);

int ncp_openmode(int fd, int exclusive);

int ncp_fcntl(int fd, int flag, void *argp);

#if defined(OS_LINUX) && defined(USE_NCPFS_LOCK)
#define	USE_NCPFS	1
#endif

#endif
