/*
	Copyright (C) 2002  ITK
	Author  : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/

#include <string.h>
#include "ci_clip.h"

#include <errno.h>
#include <fcntl.h>

#include "ci_ncp.h"

#ifdef USE_NCPFS

#include <sys/ioctl.h>
#include <unistd.h>

#include "ci_ncp_fs.h"

int
ncp_is_ncpfs(int fd)
{
   struct ncp_fs_info ncp_fs_info;

   int       r;

   memset(&ncp_fs_info, 0, sizeof(ncp_fs_info));

   ncp_fs_info.version = NCP_GET_FS_INFO_VERSION;
   r = ioctl(fd, NCP_IOC_GET_FS_INFO, &ncp_fs_info);

   if (r || !ncp_fs_info.connection)
      return 0;
   return 1;
}

int
ncp_is_ncpfs_filename(char *filename)
{
   int       fd, r;

   fd = open(filename, O_RDONLY);
   if (fd < 0)
      return 0;

   r = ncp_is_ncpfs(fd);

   close(fd);
   return r;
}

int
ncp_openmode(int fd, int exclusive)
{
   int       r;

   if (!ncp_is_ncpfs(fd))
      return 0;

   r = ioctl(fd, NCP_IOC_OPENMODE, &exclusive);

  /* invalid ioctl for this kernel */
   if (r == EINVAL)
      return 0;

   return r;
}

int
ncp_fcntl(int fd, int flag, void *argp)
{
   struct ncp_lock_ioctl ncp_lock_ioctl;

   struct flock *fl;

   int       r, ret;

   switch (flag)
    {
    case F_SETLK:
    case F_SETLKW:
    case F_UNLCK:
       break;
    case F_GETLK:
    default:
       return fcntl(fd, flag, argp);
    }

   if (!ncp_is_ncpfs(fd))
      return fcntl(fd, flag, argp);

  /* do fcntl anyway */
   ret = fcntl(fd, flag, argp);
   if (ret)
      return ret;

   fl = (struct flock *) argp;

   memset(&ncp_lock_ioctl, 0, sizeof(ncp_lock_ioctl));
   switch (fl->l_type)
    {
    case F_WRLCK:
       ncp_lock_ioctl.cmd = NCP_LOCK_EX;
       break;
    case F_RDLCK:
       ncp_lock_ioctl.cmd = NCP_LOCK_SH;
       break;
    case F_UNLCK:
       ncp_lock_ioctl.cmd = NCP_LOCK_CLEAR;
       break;
    }

   ncp_lock_ioctl.origin = 0;
   ncp_lock_ioctl.offset = fl->l_start;
   ncp_lock_ioctl.length = fl->l_len;
   ncp_lock_ioctl.timeout = NCP_LOCK_DEFAULT_TIMEOUT;

  /* and do ncpfs-specific ioctl */

   for (;;)
    {
       r = ioctl(fd, NCP_IOC_LOCKUNLOCK, &ncp_lock_ioctl);
#if 0
       printf("\nncp ioctl: cmd %d, off 0x%x, len 0x%x, r %d, errno %d",
	      ncp_lock_ioctl.cmd, ncp_lock_ioctl.offset, ncp_lock_ioctl.length, r, errno);
#endif
       if (!r)
	  return 0;

       if (errno == EAGAIN && fl->l_type == F_SETLKW)
	{
	   sleep(1);
	   continue;
	}
       break;
    }

   return r;
}

#else

int
ncp_is_ncpfs(int fd)
{
   return -1;
}

int
ncp_is_ncpfs_filename(char *filename)
{
   return -1;
}

int
ncp_openmode(int fd, int exclusive)
{
   return 0;
}

int
ncp_fcntl(int fd, int flag, void *argp)
{
   return fcntl(fd, flag, argp);
}

#endif
