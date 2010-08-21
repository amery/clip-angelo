/*
	Copyright (C) 2002  ITK
	Author  : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/

#include <string.h>
#include <ci_clip.h>

#include <errno.h>
#include <fcntl.h>

#include <ci_ncp.h>

#ifdef USE_NCPFS

#include <sys/ioctl.h>
#include <unistd.h>

#include <ci_ncp_fs.h>

#include <ncp/USE_NCPFS_ncp_is_ncpfs.c>
#include <ncp/USE_NCPFS_ncp_is_ncpfs_filename.c>
#include <ncp/USE_NCPFS_ncp_openmode.c>
#include <ncp/USE_NCPFS_ncp_fcntl.c>

#else

#include <ncp/ncp_is_ncpfs.c>
#include <ncp/ncp_is_ncpfs_filename.c>
#include <ncp/ncp_openmode.c>
#include <ncp/ncp_fcntl.c>

#endif
