/*
	Copyright (C) 2001  ITK
	Author   : Sergey Rosenfeld <serg@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include "ci_clipcfg.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>

#include <sys/param.h>
#ifndef OS_MINGW
#include <sys/mount.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>

#include <utime.h>

#include "ci_clip.h"
#include "ci_error.ch"

#ifdef HAVE_STATVFS_H
#include <sys/statvfs.h>
#undef statfs
#define statfs statvfs
#else
#ifdef HAVE_VFS_H
#include <sys/vfs.h>
#endif
#endif
#ifdef OS_CYGWIN
#include <w32api/windows.h>
#include <sys/cygwin.h>
#endif

#include "ci_hashcode.h"

/*
 * ��� �������
 */
#define PR(ptr)	printf( "'%s'\n", ptr ? ptr : "null" )

/*
 * ��� ������ hash-������ ��� ������� ������� ��������� �� ����
 * ��������� ������
 */
long      _hash_cur_dir[26];

/*
 * ��������� �������� ������
 *
 */
#define FA_NORMAL	0
#define FA_READONLY	1
#define FA_HIDDEN	2
#define FA_SYSTEM	4
#define FA_VOLUME	8
#define FA_DIRECTORY	16
#define FA_ARCHIVE	32

/* Compatibility mode.  Here DOS or the network software itself
 * determines the share mode.  In Novell networks, the SHAREABLE
 * attribute plays an important role in this relationship. */
#define LOCK_OS	0
/* Both read and write by other programs are locked out. */
#define LOCK_RDWR	1
/* Write by other programs are locked out. */
#define LOCK_WRONLY	2
/* Read by other programs are locked out. */
#define LOCK_RDONLY	3
/* Not locked.  Read and write by other programs is allowed */
#define LOCK_NONE	4

/* ���� ������ �������� ������ �� ��������� */
#define CLIP_CA_FCREATE_ATTR	0x3fffffd1
/* ���� ������ �������� ������ �� ��������� */
#define CLIP_CA_SHARE_MODE	0x3fffffd2

const char *inv_arg = "invalid argument";

/*
 * - ����� ����������� ������ hash-����� ��� ������� ������� ��������
 *   ���� �� ������� ����� (���� - ������ 'x_CUR_DIR', ��� ������ 'x'
 *   ������������� ����� �������� �����), ������������ ������ hash-�����
 *   ����� ��������� ��� <���_�����_�����> - 65
 * - ��������������� ��� �� ������������ ����� "C:" ����� � ��������
 *   ������� unix
 * - �� ����� "C:" ��������������� ������� ������� ������� -- �������� ("\")
 */

int
clip_INIT__CTOOLS_DISKFUNC(ClipMachine * ClipMachineMemory)
{
   long      i, j;

   char     *s = NULL, hstr[10];

   int      *f_attr = NULL;

#ifdef _WIN32
   LPTSTR    currpath;

   LPTSTR    savepath;

   LPTSTR    cDisk;
#else
   long      len;
#endif
  /*
   * ���������� ������� _hash_cur_dir ��� �������� ��������
   * ������� � ������� ����� �� ��������� ������ � ���������
   * ��������� ����� ��� ���� ������ � ������
   */
   strcpy(hstr, "__CUR_DIR");
   for (i = 0, j = 'A'; i < 26; i++, j++)
    {
       hstr[0] = (char) j;
       _hash_cur_dir[i] = _clip_hashstr(hstr);
       s = (char *) calloc(1, 2);
       *s = '\\';
       _clip_store_item(ClipMachineMemory, _hash_cur_dir[i], s);
    }

  /*
   * ��������� ������� ������� unix, � ���� �� �������� ������������
   * ������-���� ���������� �����, ������ ���� ���� ������� � �������������
   * ������� ������� dos
   */

#ifdef _WIN32

   savepath = malloc(sizeof(LPTSTR) * (MAXPATHLEN + 1));
   i = GetCurrentDirectory(MAXPATHLEN, savepath);
   cDisk = malloc(sizeof(LPTSTR) * 3);
   cDisk[0] = savepath[0];
   if (cDisk[0] >= 'a' && cDisk[0] <= 'z')
      cDisk[0] = cDisk[0] - 32;
   cDisk[1] = ':';
   cDisk[2] = 0;
  /* set current drive for translation path */
   _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, cDisk);

   cDisk = malloc(sizeof(LPTSTR) * 3);
   cDisk[1] = ':';
   cDisk[2] = 0;
   currpath = malloc(sizeof(LPTSTR) * (MAXPATHLEN + 1));
   for (i = 'C'; i <= 'Z'; i++)
    {
       cDisk[0] = i;
       if (SetCurrentDirectory(cDisk) <= 0)
	  continue;
       j = GetCurrentDirectory(MAXPATHLEN, currpath);
#if 0
       if (j > 3)
	{
	   char     *ch = (char *) calloc(MAXPATHLEN, 1);

	   cygwin_win32_to_posix_path_list(currpath, ch);
	   _clip_store_item(ClipMachineMemory, _hash_cur_dir[i - 65], ch);
	}
       else if (j == 3)
	{
	   char     *ch = (char *) calloc(1, 1);

	   _clip_store_item(ClipMachineMemory, _hash_cur_dir[i - 65], ch);
	}
#else
       if (j > 0)
	{
	   char     *ch = (char *) calloc(j - 1, 1);

	   memcpy(ch, currpath + 2, j - 1);
	   _clip_store_item(ClipMachineMemory, _hash_cur_dir[i - 65], ch);
	}
#endif
    }
   i = SetCurrentDirectory(savepath);
   free(savepath);
   free(currpath);
   free(cDisk);

#else
   if ((s = (char *) calloc(MAXPATHLEN, 1)))
    {

      /* ������ ������� ���� unix */
       if (getcwd(s, MAXPATHLEN))
	{
	   char     *d = NULL, *path = strdup(s);

	   hstr[1] = ':';
	   hstr[2] = 0;
	   len = strlen(s);

	  /* ������� ���� ��������� ������ */
	   for (i = 'A'; i <= 'Z'; i++)
	    {
	       hstr[0] = i;
	       if ((d = _clip_fetch_item(ClipMachineMemory, _clip_hashstr(hstr))) == NULL)
		  continue;
	       if ((j = strlen(d)) > len)
		  continue;
	       if (!memcmp(path, d, j))
		{
		   char     *ch = (char *) calloc(3, 1);

		   *ch = i;	/* ����� ����� */
		   *(ch + 1) = ':';

		  /* ��������� �������� ����� */
		  /*
		     if (_clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE) == NULL)
		   */
		   _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, ch);

		   if (path[j] == '/')
		      j++;
		   _clip_translate_path(ClipMachineMemory, path + j, s, MAXPATHLEN);
		   ch = (char *) calloc(len - j + 2, 1);
		   *ch = '\\';
		   memcpy(ch + 1, path + j, len - j);

		  /* ��������� �������� ���������� ���� */
		   _clip_store_item(ClipMachineMemory, _hash_cur_dir[i - 65], ch);

		  //   break;
		}
	    }
	   free(path);
	}
       free(s);
    }

  /*
   * ��������� ������� ���������� � ������� ��������� �����, ���� �� ���,
   * �� ������������� ������� C:
   */
#endif
   if ((s = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE)) == NULL)
    {
       s = (char *) calloc(1, 3);
       memcpy(s, "C:", 3);
       _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, s);
    }

   i = *s - 65;			/* ������ � _hash_cur_dir ��� �������� ����� */
   j = _clip_hashstr(s);	/* hash-��� ����� �������� ����� */
  /*
   * ��������� ����������� �� ����� �������� ���������� ���� � unix,
   * ���� �� ���, ������������� � �������� ������� unix
   */
   if ((s = _clip_fetch_item(ClipMachineMemory, j)) == NULL)
    {
       s = (char *) calloc(1, 2);
       *s = '/';
       _clip_store_item(ClipMachineMemory, j, s);	/* set( "?:", "/" ) */
    }

  /*
   * ������� ��� �������� ������ �� ���������
   */
   f_attr = (int *) malloc(sizeof(int));

   *f_attr = FA_ARCHIVE;
   _clip_store_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR, f_attr);

  /*
   * ����� �������� ������ �� ���������
   */
   f_attr = (int *) calloc(1, sizeof(int));

   *f_attr = LOCK_RDONLY;
   _clip_store_item(ClipMachineMemory, CLIP_CA_SHARE_MODE, f_attr);

   return 0;
}

int
clip_INI__CTOOLS_DISKFUNC(ClipMachine * ClipMachineMemory)
{
   return clip_INIT__CTOOLS_DISKFUNC(ClipMachineMemory);
}

/*
 * ���������� ��� �������� �� ������� ���� � �������� ������� unix,
 * ������� ��� ���� �������, ��� ����������� �����������
 */

char     *
_get_path(const char *fname)
{
   char     *path = (char *) malloc(MAXPATHLEN);

   int       i = 0, j = 0;

   for (; fname[i]; i++)
    {
       if ((path[i] = fname[i]) == '/')
	  j = i;
    }
   if (!j)
    {
       free(path);
       return NULL;
    }
   path[j] = 0;
   return path;
}

/*
 * ���������� ��� ������� �������� � �������, �������������
 * ��� Clipper-������ � ����������� �� errno � unix,
 * ��� ����������� �����������
 */

void
_check_error(ClipMachine * ClipMachineMemory, const char *fname, int isdir)
{
#define NO_DISK_ERR		0
#define ER_FILE_NOT_FOUND	-2
#define ER_PATH_NOT_FOUND	-3
#define ER_ACCESS_DENIED	-5

   char     *path = NULL, *tmp;

   switch (errno)
    {
    case ENOENT:
       path = _get_path(fname);
       if (isdir)
	  tmp = (char *) fname;
       else
	  tmp = path;
       if (tmp == NULL || access(tmp, F_OK))
	  _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
       else
	  _clip_retni(ClipMachineMemory, ER_FILE_NOT_FOUND);
       if (path != NULL)
	  free(path);
       return;
    case ENOTDIR:
       _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
       return;
    default:
       _clip_retni(ClipMachineMemory, ER_ACCESS_DENIED);
    }
   return;
}

/*
 * ���������� ��� ����� � �������� ������� unix, ������� ���� �������
 */

char     *
_get_unix_name(ClipMachine * ClipMachineMemory, char *dname)
{
   char     *buf = NULL;

   if (dname == NULL)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");*/
       return NULL;
    }
   if (!(buf = (char *) calloc(MAXPATHLEN, 1)))
    {
       _clip_trap_err(ClipMachineMemory, EG_MEM, 0, 0, __FILE__, __LINE__, "cannot allocate memory");
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "cannot allocate memory");*/
       return NULL;
    }
   _clip_translate_path(ClipMachineMemory, dname, buf, MAXPATHLEN);
   return buf;
}

/*
 * ���������� ���� � �������� ������� - ������ ���������� ����� dos
 */
char     *
_get_disk_path(ClipMachine * ClipMachineMemory, char *dname)
{
   char      drv[3] = { 0, ':', 0 };

   if (dname == NULL || (*(dname + 1) != 0 && *(dname + 1) != ':'))
      return NULL;

   switch (*dname)
    {
    case 'a'...'z':
       drv[0] = toupper(*dname);
       break;
    case 'A'...'Z':
       drv[0] = *dname;
       break;
    }

   if (!drv[0])
      return NULL;
   return _clip_fetch_item(ClipMachineMemory, _clip_hashstr(drv));
}

extern int _set_lock(int fd, mode_t mode);

/*extern int _check_lock( int fd, int l_type );*/

/*
 * ������� ����
 */
int
clip_DELETEFILE(ClipMachine * ClipMachineMemory)
{
/*
   0      NO_DISK_ERR         No error occurs
   -2      ER_FILE_NOT_FOUND   File not found
   -3      ER_PATH_NOT_FOUND   Path not found
   -5      ER_ACCESS_DENIED    Access denied (e.g., file is read-only)
 */
   char     *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   if (uname == NULL)
    {
       _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
       return 1;
    }
   if (unlink(uname))
      _check_error(ClipMachineMemory, uname, 0);	/* fail */
   else
      _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
   free(uname);
   return 0;
}

/*
 * ������� ������� ������� �������
 * ���� � ��������� ���� ����� �����, �� �������� � ������� ����
 */
int
clip_DIRCHANGE(ClipMachine * ClipMachineMemory)
{
/*
	 0      NO_DISK_ERR         No error occurred
	-2      ER_FILE_NOT_FOUND   File not found
	-3      ER_PATH_NOT_FOUND   Path not found
	-5      ER_ACCESS_DENIED    Access denied (e.g., in network)
*/
   char      tmp[MAXPATHLEN], *uname;

   char     *drv = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   long      hash_dir = _hash_cur_dir[*drv - 65];

   char     *dname = _clip_parc(ClipMachineMemory, 1);

   _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
   if (dname == NULL)
      return 0;

#ifdef _WIN32
   if (SetCurrentDirectory(dname) <= 0)
      return 0;
#endif
   if (strlen(dname) > 2 && *(dname + 1) == ':')
    {
       drv = calloc(1, 3);
       drv[0] = toupper(*dname);
       if (drv[0] < 'A' || drv[0] > 'Z')
	  drv[0] = 'C';
       drv[1] = ':';
      /*
         _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, drv);
       */
       hash_dir = _hash_cur_dir[*drv - 65];
       dname += 2;
    }
#ifdef _WIN32
   {
      char      ndir[MAXPATHLEN];

      char     *ch;

      int       len;

      len = GetCurrentDirectory(MAXPATHLEN, ndir);
      if (len > 0)
       {
	  ch = (char *) calloc(len - 1, 1);
	  memcpy(ch, ndir + 2, len - 1);
       }
      else
	 ch = (char *) calloc(1, 1);
      _clip_store_item(ClipMachineMemory, hash_dir, ch);
      _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
      return 0;
   }
#endif
   if (*dname == '\\' || *dname == '/')
    {
       tmp[0] = drv[0];
       tmp[1] = ':';
       strcpy(tmp + 2, dname);
       uname = _get_unix_name(ClipMachineMemory, tmp);
    }
   else
    {
       if (*dname == '.' && (*(dname + 1) == '.'))
	{
	   int       i;

	   tmp[0] = drv[0];
	   tmp[1] = ':';
	   tmp[2] = '\\';
	   strcpy(tmp + 3, _clip_fetch_item(ClipMachineMemory, hash_dir));
	   for (i = 0; tmp[i] != 0; i++);
	   for (; i > 2 && tmp[i] != '\\' && tmp[i] != '/'; i--);
	   strcpy(tmp + i, dname + 2);
	   uname = _get_unix_name(ClipMachineMemory, tmp);
	}
       else
	{

	   tmp[0] = drv[0];
	   tmp[1] = ':';
	  /*snprintf(tmp + 2,MAXPATHLEN,"%s\\%s",_clip_fetch_item(ClipMachineMemory, hash_dir), dname); */
	   strcpy(tmp + 2, _clip_fetch_item(ClipMachineMemory, hash_dir));
	   strcat(tmp, "\\");
	   strcat(tmp, dname);
	   uname = _get_unix_name(ClipMachineMemory, tmp);
	  //printf("\n\ndname=%s,uname=%s\n",dname,uname);
	}
    }

   if (uname == NULL)
    {
       _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
       return 1;
    }

  //printf("\nunix_dir=%s\n",uname);
   _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
   if (chdir(uname))
    {
       _check_error(ClipMachineMemory, uname, 1);	/* fail */
    }
   else
    {
      /*
       * ����� �������� unix �������, ������ ������������� �������
       * ���� (���� ����) � ������� ��������� ���� �� ���� �����
       */
       char     *dir = NULL, *ndir = NULL;

       if (*dname == '\\' || *dname == '/')
	{			/* ���������� ���� dos */
	   int       len;

	   drv = strdup(dname);
	   len = strlen(drv);
	   if (len > 3 && (drv[len - 1] == '/' || drv[len - 1] == '\\'))
	      drv[len - 1] = 0;
	   _clip_store_item(ClipMachineMemory, hash_dir, drv);
	}
       else
	{			/* ���� ������������, ������� ����� */
	   int       dlen = 0;

	   dir = _clip_fetch_item(ClipMachineMemory, hash_dir);
	   dlen = strlen(dir);
	   ndir = (char *) calloc(dlen + strlen(dname) + 2, 1);
	   memcpy(ndir, dir, dlen);
	   if (*dname == '.' && (*(dname + 1) == '.'))
	    {
	       int       i;

	       for (i = 0; ndir[i] != 0; i++);
	       for (; i > 0 && ndir[i] != '\\' && ndir[i] != '/'; i--);

	       if (dname[2] == '\\' || dname[2] == '/')
		  strcpy(ndir + i, dname + 3);
	       else
		  strcpy(ndir + i, dname + 2);
	    }
	   else
	    {
	       if (dlen > 1)
		  ndir[dlen++] = '\\';
	       strcpy(ndir + dlen, dname);
	    }
	   dlen = strlen(ndir);
	   if (dlen > 3 && (ndir[dlen - 1] == '/' || ndir[dlen - 1] == '\\'))
	      ndir[dlen - 1] = 0;
	   _clip_store_item(ClipMachineMemory, hash_dir, ndir);
	}

       _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
    }

   free(uname);
   return 0;
}

/*
 * ������� �������, � ���� ����� ���� ����� �����
 */
int
clip_DIRMAKE(ClipMachine * ClipMachineMemory)
{
/*
   0      NO_DISK_ERR         No error occurred
   -2      ER_FILE_NOT_FOUND   File not found
   -3      ER_PATH_NOT_FOUND   Path not found
   -5      ER_ACCESS_DENIED    Access denied (e.g., in network)
 */
   char     *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   if (uname == NULL)
    {
       _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
       return 1;
    }

  /*
   * ������� ������� �� ����� ������� ��� ����, ��� ���������
   * � ����������� �� ��������� umask ��� ����� �����
   */
#ifdef OS_MINGW
   if (mkdir(uname))
#else
   if (mkdir(uname, ClipMachineMemory->dirCreateMode))
#endif
      _check_error(ClipMachineMemory, uname, 1);	/* fail */
   else
      _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */

   free(uname);
   return 0;
}

/*
 * ��� �������� �������� ��������, �������� 1 �������� -- ��� �����,
 * ���� ��� ���, �� ������� ������� ����
 */
int
clip_DIRNAME(ClipMachine * ClipMachineMemory)
{
   char     *buf = (char *) malloc(MAXPATHLEN);

   char     *dname = _clip_parc(ClipMachineMemory, 1);

   char     *drv = dname ? dname : _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   char     *dir = _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[toupper(*drv) - 65]);

   int       i = 0, n = 0;

   if (dir == NULL)
    {
       buf[0] = '\\';
       buf[1] = 0;
    }
   else
    {
       for (i = 0, n = 0; dir[i]; i++, n++)
	  buf[n] = (dir[i] == '/' ? '\\' : dir[i]);
       buf[n] = 0;
    }
   _clip_retcn_m(ClipMachineMemory, buf, strlen(buf));
   return 0;
}

/*
 * �������� ��������, �������� �������� �� ������� �������,
 * �.�. ��� �� ��������� �������� ������� ������
 */
int
clip_DIRREMOVE(ClipMachine * ClipMachineMemory)
{
/*
   0       Directory has been successfully removed.
   -3      Access path not found or directory not empty.
   -5      Access denied (e.g., in a network)
   -16     Current directory (cannot be removed)
 */
   struct stat st, stcur;

   char     *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   if (uname == NULL)
    {
       _clip_retni(ClipMachineMemory, -3);
       return 1;
    }

   if (stat(uname, &st))
    {
       _clip_retni(ClipMachineMemory, errno == ENOENT ? -3 : -5);
       goto ret;
    }

   if (stat(".", &stcur))
    {
       _clip_retni(ClipMachineMemory, -5);
       goto ret;
    }

  /*
   * �������� �� ������� �������
   */
   if (st.st_ino == stcur.st_ino)
    {
       _clip_retni(ClipMachineMemory, -16);
       goto ret;
    }

   if (!rmdir(uname))		/* success */
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, -3);

 ret:
   free(uname);
   return 0;
}

/*
 * ����� �������� �����, (A, B, C, D, ...),
 * ����� ����� ���� � ������������ �����������
 */
int
clip_DISKCHANGE(ClipMachine * ClipMachineMemory)
{
   struct stat st;

   char     *drv = NULL;

   char     *dname = _clip_parc(ClipMachineMemory, 1);

   char     *uname = _get_disk_path(ClipMachineMemory, dname);

   if (uname == NULL || stat(uname, &st) || !S_ISDIR(st.st_mode))
    {
       _clip_retl(ClipMachineMemory, 0);
       return 0;
    }

   drv = (char *) calloc(3, 1);
   drv[1] = ':';
   switch (*dname)
    {
    case 'a'...'z':
       drv[0] = toupper(*dname);
       break;
    case 'A'...'Z':
       drv[0] = *dname;
       break;
    }
#ifdef _WIN32
   if (SetCurrentDirectory(drv) > 0)
    {
       _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, drv);
       _clip_retl(ClipMachineMemory, 1);
    }
   else
      _clip_retl(ClipMachineMemory, 0);
#else
   _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, drv);
   _clip_retl(ClipMachineMemory, 1);
#endif
   return 0;
}

char     *
_clip_curdir(ClipMachine * ClipMachineMemory, char *drv)
{
   int       i;

   char     *dir;

   dir = _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[drv[0] - 65]);
   if (dir == NULL /* || *dir==0 */ )
      return "";
   for (i = 0; dir[i] != 0; i++)
      if (dir[i] == '\\')
	 dir[i] = '/';
   if (*dir == '/')
      dir++;
   return dir;
}

int
clip_CURDIR(ClipMachine * ClipMachineMemory)
{
   char      drv[3], *tmp;

   char     *disk = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
    {
       tmp = _clip_parc(ClipMachineMemory, 1);
       drv[0] = toupper(*tmp);
    }
   else
      drv[0] = *disk;
   drv[1] = ':';
   drv[2] = 0;
   _clip_retc(ClipMachineMemory, _clip_curdir(ClipMachineMemory, drv));
   return 0;
}

int
clip_DISKFREE(ClipMachine * ClipMachineMemory)
{
   char     *dname = _clip_parc(ClipMachineMemory, 1);

   char     *uname;

#ifdef OS_MINGW
   DWORD     fs;

   if (dname == NULL || *dname == 0)
      dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   uname = _get_disk_name(ClipMachineMemory, dname);
   GetDiskFreeSpace(uname, NULL, NULL, &fs, NULL);
   _clip_retnd(ClipMachineMemory, ((double) fs) * fs);
#else
   struct statfs st;

   char      drv[3];

   char     *tmp, buf[PATH_MAX];

   if (dname == NULL || *dname == 0)
      dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   if (*dname == '/')
      uname = dname;
   else
    {
       drv[0] = *dname;
       drv[1] = ':';
       drv[2] = 0;
       tmp = _clip_curdir(ClipMachineMemory, drv);

       buf[0] = drv[0];
       buf[1] = ':';
       buf[2] = '/';
       strncpy(buf + 3, tmp, PATH_MAX - 4);
       uname = _get_unix_name(ClipMachineMemory, buf);
    }

   if (uname == NULL)
      uname = dname;

   if (uname == NULL)
    {
       _clip_retnd(ClipMachineMemory, 0);
       return 0;
    }

   if (statfs(uname, &st))
    {
       _clip_retnd(ClipMachineMemory, 0);
       return 0;
    }
   _clip_retnd(ClipMachineMemory, ((double) st.f_bavail) * st.f_bsize);
#endif

   return 0;
}

int
clip_DISKNAME(ClipMachine * ClipMachineMemory)
{
   char      drv[2] = { 0, 0 };
   char     *dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   if (dname == NULL)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "unknown disk");
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "unknown disk");*/
       _clip_retc(ClipMachineMemory, "");
       return 1;
    }
   drv[0] = *dname;
   _clip_retc(ClipMachineMemory, drv);
   return 0;
}

int
clip_DISKREADY(ClipMachine * ClipMachineMemory)
{
   struct stat st;

   char     *dname = _clip_parc(ClipMachineMemory, 1);

   char     *uname = _get_disk_path(ClipMachineMemory, dname);

   if (dname == NULL || *dname == 0)
    {
       dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);
       uname = _get_disk_path(ClipMachineMemory, dname);
    }
   if (uname == NULL || stat(uname, &st))
      _clip_retl(ClipMachineMemory, 0);
   else
      _clip_retl(ClipMachineMemory, 1);

   return 0;
}

int
clip_DISKREADYW(ClipMachine * ClipMachineMemory)
{
   struct stat st;

   char     *dname = _clip_parc(ClipMachineMemory, 1);

   char     *uname = _get_disk_path(ClipMachineMemory, dname);

   if (dname == NULL || *dname == 0)
    {
       dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);
       uname = _get_disk_path(ClipMachineMemory, dname);
    }
   if (uname == NULL || stat(uname, &st))
    {
       _clip_retl(ClipMachineMemory, 0);
    }
   else
    {
#ifdef OS_MINGW
       _clip_retl(ClipMachineMemory, 0);
#else
       uid_t     uid = geteuid();

       gid_t     gid = getegid();

       if ((uid == st.st_uid && (st.st_mode & S_IWUSR)) ||
	   (gid == st.st_gid && (st.st_mode & S_IWGRP)) || (st.st_mode & S_IWOTH))
	{
	   _clip_retl(ClipMachineMemory, 1);
	}
       else
	{
	   _clip_retl(ClipMachineMemory, 0);
	}
#endif
    }
   return 0;
}

int
clip_DISKTOTAL(ClipMachine * ClipMachineMemory)
{
#ifdef OS_MINGW
   DWORD     dt;
#else
   struct statfs st;
#endif
   char     *dname = _clip_parc(ClipMachineMemory, 1);

   char     *uname;

   if (dname == NULL || *dname == 0)
      dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   uname = _get_disk_path(ClipMachineMemory, dname);

   if (uname == NULL)
      uname = dname;

   if (uname == NULL)
    {
       _clip_retnd(ClipMachineMemory, 0);
       return 0;
    }

#ifdef OS_MINGW
   GetDiskFreeSpace(uname, NULL, NULL, NULL, &dt);
   _clip_retnd(ClipMachineMemory, (double) dt);
#else
   if (statfs(uname, &st))
    {
       _clip_retnd(ClipMachineMemory, 0);
       return 0;
    }
   _clip_retnd(ClipMachineMemory, ((double) st.f_blocks) * st.f_bsize);
#endif

   return 0;
}

int
clip_DISKTYPE(ClipMachine * ClipMachineMemory)	/* Determines the type of data carrier */
{
/*
   255     DT_DS_SEC_8         Double sided 8 sectors
   254     DT_SS_SEC_8         Single sided 8 sectors
   253     DT_DS_SEC_9         Double sided 9 sectors
   252     DT_SS_SEC_9         Single sided 9 sectors
   249     DT_DS_SEC_15        Double sided 15 sectors (HD-Disk)
   249     DT_35_SEC_9         Double sided 3.5" 9 sectors (ident. to 5.25" HD)
   240     DT_35_SEC_18        Double sided 3.5" 18 sectors
   248     DT_HARDDISK         Hard disk
 */
#ifdef _WIN32
   LPTSTR    path = _clip_parc(ClipMachineMemory, 1);

   LPTSTR    buf;

   int       dType;

   int       ret = 0, len;

   if (path == NULL || (*path) == 0)
      path = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   len = strlen(path);
   switch (len)
    {
    case 1:
       buf = malloc(4 * sizeof(LPTSTR));
       buf[0] = path[0];
       buf[1] = ':';
       buf[2] = '/';
       buf[3] = 0;
       break;
    case 2:
       buf = malloc(4 * sizeof(LPTSTR));
       buf[0] = path[0];
       buf[1] = path[1];
       buf[2] = '/';
       buf[3] = 0;
       break;
    default:
       buf = malloc(len + 1);
       memcpy(buf, path, len);
       ((char *) buf)[len] = 0;
       break;
    }
   dType = GetDriveType(buf);
   free(buf);

   switch (dType)
    {
    case 0:
    case 1:
       break;
    case DRIVE_RAMDISK:
       ret = 254;
       break;
    case DRIVE_FIXED:
    case DRIVE_REMOTE:
    case DRIVE_CDROM:
       ret = 248;
       break;
    case DRIVE_REMOVABLE:
       ret = 249;
       break;

    }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
#else
   _clip_retni(ClipMachineMemory, 248);
   return 0;
#endif
}

int
clip_DRIVETYPE(ClipMachine * ClipMachineMemory)	/* Determines the drive type */
{
/*
   0       RAM Disk
   1       Floppy Disk (disk change cannot be established by system)
   2       Floppy Disk (disk change can be established by system)
   3       Hard Disk
 */
#ifdef _WIN32
   LPTSTR    path = _clip_parc(ClipMachineMemory, 1);

   LPTSTR    buf;

   int       dType;

   int       ret = 0, len;

   if (path == NULL || (*path) == 0)
      path = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   len = strlen(path);
   switch (len)
    {
    case 1:
       buf = malloc(4 * sizeof(LPTSTR));
       buf[0] = path[0];
       buf[1] = ':';
       buf[2] = '/';
       buf[3] = 0;
       break;
    case 2:
       buf = malloc(4 * sizeof(LPTSTR));
       buf[0] = path[0];
       buf[1] = path[1];
       buf[2] = '/';
       buf[3] = 0;
       break;
    default:
       buf = malloc(len + 1);
       memcpy(buf, path, len);
       ((char *) buf)[len] = 0;
       break;
    }
   dType = GetDriveType(buf);
   free(buf);

   switch (dType)
    {
    case 0:
    case 1:
       break;
    case DRIVE_RAMDISK:
       ret = 0;
       break;
    case DRIVE_FIXED:
    case DRIVE_REMOTE:
    case DRIVE_CDROM:
       ret = 3;
       break;
    case DRIVE_REMOVABLE:
       ret = 2;
       break;

    }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
#else
   _clip_retni(ClipMachineMemory, 3);
   return 0;
#endif
}

int
clip_FILEAPPEND(ClipMachine * ClipMachineMemory)	/* Appends data to a file */
{
   char     *src = _clip_parc(ClipMachineMemory, 1);

   char     *dst = _clip_parc(ClipMachineMemory, 2);

   char     *usrc = _get_unix_name(ClipMachineMemory, src);

   char     *udst = _get_unix_name(ClipMachineMemory, dst);

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int      *fattr = _clip_fetch_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR);

   mode_t    mode = ClipMachineMemory->fileCreateMode;

   int       fdsrc = -1, fddst = -1;

   FILE     *fsrc = NULL;

   FILE     *fdst = NULL;

   int       c = 0;

   off_t     i = 0;

   struct stat st;

   *err = 0;

   if (src == NULL || dst == NULL || *src == 0 || *dst == 0 || usrc == NULL || udst == NULL)
    {
       _clip_retni(ClipMachineMemory, 0);
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
       return 1;
    }

   if (fattr && !(*fattr & FA_READONLY))
    {
#ifdef OS_MINGW
       mode |= S_IWUSR;
#else
       mode |= S_IWUSR | S_IWGRP | S_IWOTH;
#endif
    }

   if ((fdsrc = open(usrc, O_RDONLY)) < 0)
    {
     err:
       _clip_retni(ClipMachineMemory, 0);
       *err = errno;
       goto end;
    }

   if (!_set_lock(fdsrc, F_RDLCK) || fstat(fdsrc, &st))
    {
     err1:
       close(fdsrc);
       goto err;
    }

   if (access(udst, F_OK))
    {
       if ((fddst = creat(udst, mode)) < 0)
	  goto err1;
    }
   else
    {
       if ((fddst = open(udst, O_WRONLY)) < 0)
	  goto err1;
       if (!_set_lock(fddst, F_WRLCK))
	{
	 err2:
	   close(fddst);
	   goto err1;
	}
    }

   if (!(fsrc = fdopen(fdsrc, "r")))
    {
       goto err2;
    }
   if (!(fdst = fdopen(fddst, "a")))
    {
       close(fddst);
       _clip_retni(ClipMachineMemory, 0);
       *err = errno;
       goto end;
    }

   for (i = 0; (c = fgetc(fsrc)) != EOF && i < st.st_size; i++)
    {
       if (fputc(c, fdst) == EOF)
	{
	   *err = errno;
	   break;
	}
    }
   _clip_retnd(ClipMachineMemory, i);
 end:
   if (fsrc)
      fclose(fsrc);
   if (fdst)
      fclose(fdst);
   free(usrc);
   free(udst);
   return 0;
}

int
clip___COPYFILE(ClipMachine * ClipMachineMemory)
{
   char     *src = _clip_parc(ClipMachineMemory, 1);

   char     *dst = _clip_parc(ClipMachineMemory, 2);

   char     *usrc = _get_unix_name(ClipMachineMemory, src);

   char     *udst = _get_unix_name(ClipMachineMemory, dst);

   int       fdsrc = -1, fddst = -1;

   FILE     *fsrc = NULL;

   FILE     *fdst = NULL;

   int       c = 0, r = 0, ret = 0;

   int      *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   const char *funcname = "__COPYFILE";

   *err = 0;
   if (src == NULL || dst == NULL || *src == 0 || *dst == 0 || usrc == NULL || udst == NULL)
    {
       _clip_retni(ClipMachineMemory, ret);
       r = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "invalid argument");
       goto end;
    }

   if ((fdsrc = open(usrc, O_RDONLY)) < 0)
      goto end;
   else if ((fddst = creat(udst, ClipMachineMemory->fileCreateMode)) < 0)
    {
       ret = 0;
       r = _clip_trap_err(ClipMachineMemory, EG_OPEN, 0, 0, __FILE__, __LINE__, funcname);
       *err = errno;
       goto end;
    }
   if (!_set_lock(fdsrc, F_RDLCK) /*|| !_set_lock(fddst, F_WRLCK) */ )
    {
       ret = 0;
       r = _clip_trap_err(ClipMachineMemory, EG_LOCK, 0, 0, __FILE__, __LINE__, funcname);
       *err = errno;
       goto end;
    }

   ftruncate(fddst, 0);

   if ((fsrc = fdopen(fdsrc, "r")) == NULL || (fdst = fdopen(fddst, "w")) == NULL)
    {
       ret = 0;
       r = _clip_trap_err(ClipMachineMemory, EG_OPEN, 0, 0, __FILE__, __LINE__, funcname);
       *err = errno;
       goto end;
    }

   while ((c = fgetc(fsrc)) != EOF)
    {
       if (fputc(c, fdst) == EOF)
	{
	   *err = errno;
	   r = _clip_trap_err(ClipMachineMemory, EG_WRITE, 0, 0, __FILE__, __LINE__, "__COPYFILE");
	   break;
	}
       ret++;
    }

 end:
   _clip_retni(ClipMachineMemory, ret);

   if (fsrc)
      fclose(fsrc);
   else if (fdsrc >= 0)
      close(fdsrc);

   if (fdst)
      fclose(fdst);
   else if (fddst >= 0)
      close(fddst);

   if (usrc)
      free(usrc);

   if (udst)
      free(udst);

   return r;
}

/* ����� �������� ���������� ��� FILESEEK */
typedef struct
{
   char    **fname;
   struct stat *st;
   int       pos;
}
FSdata;

/* ���������� ������ FSdata */
void
destroy_FSdata(void *item)
{
   FSdata   *d = (FSdata *) item;

   int       i = 0;

   if (d == NULL)
      return;

   if (d->fname)
    {
       while (d->fname[i])
	  free(d->fname[i++]);
       free(d->fname);
    }

   if (d->st)
    {
       free(d->st);
    }

   free(d);
}

int
clip_FILESEEK(ClipMachine * ClipMachineMemory)	/* Searches for files by name and attribute */
{
/*
   0       FA_NORMAL
   1       FA_READONLY         READ ONLY (Read-only)
   2       FA_HIDDEN           HIDDEN (Hidden files)
   4       FA_SYSTEM           SYSTEM (System files)
   8       FA_VOLUME           VOLUME (Name of a floppy/hard disk)
   16      FA_DIRECTORY        DIR (Directory)
   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
 */

   char     *dname = _clip_parc(ClipMachineMemory, 1);

   int       fattr = _clip_parni(ClipMachineMemory, 2);

  /*int exact = _clip_parl(ClipMachineMemory, 3); */
   char     *uname = NULL, *mask = NULL, *fullname = NULL, *filename = NULL;

   int       i = 0, valid = 0, ret = 0;

   FSdata   *fsd = NULL;

   DIR      *dir = NULL;

   struct dirent *entry = NULL;

   if (dname)
      uname = _get_unix_name(ClipMachineMemory, dname);

   if (!uname || !*uname)
    {
       if ((fsd = _clip_fetch_item(ClipMachineMemory, HASH_fsdata)) == NULL)
	{
	   _clip_retni(ClipMachineMemory, 0);
	   ret = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
			/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");*/
	   goto end;
	}
    }

   if (fsd == NULL)
    {
       fsd = (FSdata *) calloc(sizeof(FSdata), 1);
       if ((mask = strrchr(uname, '/')) == NULL || *(mask + 1) == 0)
	{
	   mask = "*";
	}
       else
	{
	   *mask++ = 0;
	}

       if ((dir = opendir((*uname) == 0 ? "/" : uname)) != NULL)
	{
	   fullname = (char *) calloc(MAXPATHLEN, 1);
	   strcpy(fullname, uname);
	   strcat(fullname, "/");
	   filename = fullname + strlen(fullname);
	}
       else
	{
	   _clip_retni(ClipMachineMemory, 0);
	  /*
	     ret = _clip_trap_err( ClipMachineMemory, EG_OPEN, 0, 0, __FILE__, __LINE__, "cannot open directory" );
	   */
	   goto end;
	}

       for (i = 0; (entry = readdir(dir)) != NULL;)
	{
	   if (fsd->fname == NULL || fsd->fname[i] == 0)
	    {
	       fsd->fname = (char **) realloc(fsd->fname, (i + 64) * sizeof(char *));
	       fsd->st = (struct stat *) realloc(fsd->st, (i + 64) * sizeof(struct stat));

	       fsd->fname[i] = 0;
	    }
#ifdef _WIN32
	   if (_clip_glob_match(entry->d_name, mask, 1) != strlen(entry->d_name))
#else
	   if (_clip_glob_match(entry->d_name, mask, ClipMachineMemory->flags & TRANSLATE_FLAG) != strlen(entry->d_name))
#endif
	      continue;
	   strcpy(filename, entry->d_name);
	   stat(fullname, &(fsd->st[i]));
	   valid = 0;
	   if (fattr & FA_DIRECTORY)
	    {
	       if (S_ISDIR(fsd->st[i].st_mode))
		{
		   fsd->fname[i++] = strdup(entry->d_name);
		   fsd->fname[i] = 0;
		   continue;
		}
	    }
	   else
	    {
	       if (S_ISDIR(fsd->st[i].st_mode))
		  continue;
	    }
	   if (fattr == 0)
	    {
	       valid = 1;
	    }
	   else if (fattr & FA_ARCHIVE && S_ISREG(fsd->st[i].st_mode))
	      valid = 1;
	   if (fattr & FA_READONLY)
	    {
	       if ((fsd->st[i].st_mode & S_IRUSR) && !(fsd->st[i].st_mode & S_IWUSR))
		  valid = 1;
	    }
	   else if (fattr & FA_HIDDEN)
	    {
	       if (entry->d_name[0] == '.')
		  valid = 1;
	    }
	   else if (FA_SYSTEM || FA_VOLUME)
	    {
	       ;
	    }
	   else
	    {
	       valid = 1;
	    }

	   if (valid)
	    {
	       fsd->fname[i++] = strdup(entry->d_name);
	       fsd->fname[i] = 0;
	    }
	}

       closedir(dir);

       if (fsd->fname)
	  fsd->fname[i] = 0;

       _clip_store_item_destroy(ClipMachineMemory, HASH_fsdata, fsd, destroy_FSdata);

    }
   else if (fsd->fname[fsd->pos])
    {
       fsd->pos++;
    }

   _clip_retc(ClipMachineMemory, fsd->fname[fsd->pos]);

 end:
   if (uname)
      free(uname);
   if (fullname)
      free(fullname);

   return ret;
}

/*
 * ���� ���� ��� �����, �� �������������� ���,
 * ����� ������� ������� � ������ FILESEEK
 */
char     *
_get_fileseek_info(ClipMachine * ClipMachineMemory, char **fname, struct stat *st)
{
   char     *uname = ClipMachineMemory->argc > 0 ? _get_unix_name(ClipMachineMemory,
								  _clip_parc(ClipMachineMemory,
									     1)) : NULL;

   FSdata   *fsd = NULL;

   if (uname != NULL)
    {
       if (stat(uname, st))
	{
	   _clip_retni(ClipMachineMemory, 0);
	   return NULL;
	}
       if ((*fname = strrchr(uname, '/')) == NULL)
	{
	   *fname = uname;
	}
       else
	{
	   char     *tmp = *fname;

	   *fname = ++tmp;
	}
    }
   else
    {
       if ((fsd = _clip_fetch_item(ClipMachineMemory, HASH_fsdata)) == NULL)
	{
	   _clip_retni(ClipMachineMemory, 0);
	   return NULL;
	}
       *fname = fsd->fname[fsd->pos];
       *st = fsd->st[fsd->pos];
    }
   return uname;
}

int
clip_FILEATTR(ClipMachine * ClipMachineMemory)	/* Determines a file's attributes */
{
/*
   0       FA_NORMAL
   1       FA_READONLY         READ ONLY (Read-only)
   2       FA_HIDDEN           HIDDEN (Hidden files)
   4       FA_SYSTEM           SYSTEM (System files)
   8       FA_VOLUME           VOLUME (Name of a floppy/hard disk)
   16      FA_DIRECTORY        DIR (Directory)
   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
 */
   struct stat st;

   int       fattr = 0;

   char     *fname = NULL;

   char     *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   if (fname != NULL)
    {
       if (S_ISREG(st.st_mode))
	{
	   fattr += FA_ARCHIVE;
	}
       if (fname[0] == '.')
	{
	   fattr += FA_HIDDEN;
	}
       if ((st.st_mode & S_IRUSR) && !(st.st_mode & S_IWUSR))
	{
	   fattr += FA_READONLY;
	}
       if (S_ISDIR(st.st_mode))
	{
	   fattr += FA_DIRECTORY;
	}
    }

   _clip_retni(ClipMachineMemory, fattr);

   if (uname != NULL)
      free(uname);

   return 0;
}

int
clip_FILEATTRS(ClipMachine * ClipMachineMemory)	/* Determines a file's attributes */
{
/*
   0       FA_NORMAL
   1       FA_READONLY         READ ONLY (Read-only)
   2       FA_HIDDEN           HIDDEN (Hidden files)
   4       FA_SYSTEM           SYSTEM (System files)
   8       FA_VOLUME           VOLUME (Name of a floppy/hard disk)
   16      FA_DIRECTORY        DIR (Directory)
   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
 */
   struct stat st;

   char      fattr[8];

   char     *fname = NULL;

   char     *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   int       i = 0;

   if (fname != NULL)
    {
       if (S_ISREG(st.st_mode))
	{
	   fattr[i++] = 'A';
	}
       if (fname[0] == '.')
	{
	   fattr[i++] = 'H';
	}
       if ((st.st_mode & S_IRUSR) && !(st.st_mode & S_IWUSR))
	{
	   fattr[i++] = 'R';
	}
       if (S_ISDIR(st.st_mode))
	{
	   fattr[i++] = 'D';
	}
       fattr[i] = 0;
    }

   _clip_retc(ClipMachineMemory, fattr);

   if (uname != NULL)
      free(uname);

   return 0;
}

int
clip_FILEDATE(ClipMachine * ClipMachineMemory)	/* Determines the file date */
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   16      FA_DIRECTORY        DIR (Directory)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
*/
   struct stat st;

   char     *fname = NULL;

   char     *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   int       fattr = _clip_parni(ClipMachineMemory, 2);

   struct tm *t = NULL;

   if (fname != NULL && (fattr == FA_NORMAL ||
			 ((fattr & FA_READONLY) && (st.st_mode & S_IRUSR)
			  && !(st.st_mode & S_IWUSR)) || ((fattr & FA_HIDDEN)
							  && fname[0] == '.')
			 || ((fattr & FA_DIRECTORY) && S_ISDIR(st.st_mode)) || ((fattr & FA_ARCHIVE) && S_ISREG(st.st_mode))))
    {
       t = localtime(&st.st_mtime);
    }

   if (t == NULL)
      _clip_retdj(ClipMachineMemory, 0);
   else
      _clip_retdc(ClipMachineMemory, 1900 + t->tm_year, t->tm_mon + 1, t->tm_mday);

   if (uname != NULL)
      free(uname);

   return 0;
}

int
clip_FILESIZE(ClipMachine * ClipMachineMemory)	/* Determines the size of a file */
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup.)
*/
   struct stat st;

   char     *fname = NULL;

   char     *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   int       fattr = _clip_parni(ClipMachineMemory, 2);

   off_t     fsize = -1;

   if (fname != NULL && (fattr == FA_NORMAL ||
			 ((fattr & FA_READONLY) && (st.st_mode & S_IRUSR)
			  && !(st.st_mode & S_IWUSR)) || ((fattr & FA_HIDDEN)
							  && fname[0] == '.')
			 || ((fattr & FA_DIRECTORY) && S_ISDIR(st.st_mode)) || ((fattr & FA_ARCHIVE) && S_ISREG(st.st_mode))))
    {
       fsize = st.st_size;
    }

   _clip_retnl(ClipMachineMemory, fsize);

   if (uname != NULL)
      free(uname);

   return 0;
}

int
clip_FILETIME(ClipMachine * ClipMachineMemory)	/* Determines a file's time */
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   16      FA_DIRECTORY        (Subdirectory)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
*/
   struct stat st;

   char     *fname = NULL;

   char     *uname = _get_fileseek_info(ClipMachineMemory, &fname, &st);

   int       fattr = _clip_parni(ClipMachineMemory, 2);

   struct tm *t = NULL;

   char      tbuf[9];

   memset(tbuf, 0, 9);

   if (fname != NULL && (fattr == FA_NORMAL ||
			 ((fattr & FA_READONLY) && (st.st_mode & S_IRUSR)
			  && !(st.st_mode & S_IWUSR)) || ((fattr & FA_HIDDEN)
							  && fname[0] == '.')
			 || ((fattr & FA_DIRECTORY) && S_ISDIR(st.st_mode)) || ((fattr & FA_ARCHIVE) && S_ISREG(st.st_mode))))
    {
       t = localtime(&st.st_mtime);
       snprintf(tbuf, 9, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
    }

   _clip_retc(ClipMachineMemory, tbuf);

   if (uname != NULL)
      free(uname);

   return 0;
}

int
clip_SAVEFSEEK(ClipMachine * ClipMachineMemory)	/* Saves the current FILESEEK environment */
{
   return 0;
}

int
clip_RESTFSEEK(ClipMachine * ClipMachineMemory)	/* Restores the FILESEEK environment */
{
   return 0;
}

int
clip_SETFATTR(ClipMachine * ClipMachineMemory)	/* Sets a file's attributes */
{
/*
	   ������������ ��������:
	   ~~~~~~~~~~~~~~~~~~~~~~
	0      NO_DISK_ERR         No error found
	   -2      ER_FILE_NOT_FOUND   File not found
	   -3      ER_PATH_NOT_FOUND   Path not found
	   -5      ER_ACCESS_DENIED    Access denied (e.g., in network)

	   ��������������� ��������:
	   ~~~~~~~~~~~~~~~~~~~~~~~~~
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
*/
   char     *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   int       fattr = _clip_parni(ClipMachineMemory, 2);

   mode_t    mode = 0;

   if (uname == NULL)
    {
       _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
       return 1;
    }
#ifdef OS_MINGW
   mode = S_IRUSR;
#else
   mode = S_IRUSR | S_IRGRP | S_IROTH;
#endif
   if (!(fattr & FA_READONLY))
    {
#ifdef OS_MINGW
       mode = S_IWUSR;
#else
       mode |= S_IWUSR | S_IWGRP | S_IWOTH;
#endif
    }
   if (chmod(uname, mode) != 0)
    {
       _check_error(ClipMachineMemory, uname, 0);	/* fail */
    }
   else
    {
       _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
    }
   free(uname);
   return 0;
}

/*
 *      FILESTR(<cFile>, [<nLength>], [<nOffset>],
 *	  [<lCtrl-Z>]) --> cCharacterstring
 */
int
clip_FILESTR(ClipMachine * ClipMachineMemory)	/* Reads a portion of a file into a string */
{
   char     *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   char     *ret = NULL;

   long      nlen = _clip_parnl(ClipMachineMemory, 2);

   long      noff = _clip_parnl(ClipMachineMemory, 3);

   long      n = 0;

   int       c = 0, ctrl_z = _clip_parl(ClipMachineMemory, 4);

   int       fd = -1;

   FILE     *f = NULL;

   if (uname == NULL || noff < 0 || nlen < 0)
      goto end;

   if (nlen == 0)
      nlen = 0xFFFF;

   if ((fd = open(uname, O_RDONLY)) == -1 || !_set_lock(fd, F_RDLCK) ||
       (f = fdopen(fd, "r")) == NULL || fseek(f, noff, SEEK_SET) || (ret = (char *) malloc(nlen)) == NULL)
      goto end;

   if (!ctrl_z)
    {
       n = fread(ret, 1, nlen, f);
    }
   else
      while ((c = fgetc(f)) != EOF)
       {
	  if (n >= nlen || c == 26 /* ctrl_z */ )
	     break;
	  ret[n++] = c;
       }

   fclose(f);
 end:
   if (uname != NULL)
      free(uname);

   _clip_retcn_m(ClipMachineMemory, ret, (ret ? n : 0));

   return 0;
}

/*
 * Default attribute for creating with CA-Clipper Tools functions
 */
int
clip_SETFCREATE(ClipMachine * ClipMachineMemory)
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY (Read-only)
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
*/
   int       oldattr = -1, *ptrattr = NULL;

   int       newattr = _clip_parni(ClipMachineMemory, 1);

   if (newattr < 0)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
       return 1;
    }
   ptrattr = _clip_fetch_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR);
   oldattr = *ptrattr;

   ptrattr = (int *) malloc(sizeof(int));

   *ptrattr = newattr;
   _clip_store_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR, ptrattr);

   _clip_retni(ClipMachineMemory, oldattr);

   return 0;
}

/*
 *      STRFILE(<cCharacterstring>, <cFile>, [<lOverwrite>],
 *	  [<nOffset>], [<lCutOff>]) --> nRecordedByte
 */
int
clip_STRFILE(ClipMachine * ClipMachineMemory)	/* Writes a string to a file */
{
   const char *str = _clip_parc(ClipMachineMemory, 1);

   char     *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2));

   int       lowr = _clip_parl(ClipMachineMemory, 3);

   long      n = 0, noff = _clip_parnl(ClipMachineMemory, 4);

   int       lcut = _clip_parl(ClipMachineMemory, 5);

   int       fd = -1, flags = O_WRONLY | O_CREAT;

   mode_t    mode = ClipMachineMemory->fileCreateMode;

   int      *fattr = _clip_fetch_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR);

   if (lowr == 0)		/* create new file */
    {
       flags |= O_CREAT;
       unlink(uname);
    }
   else if (ClipMachineMemory->argc <= 3)
    {				/* nOffset - undefined */
       flags |= O_APPEND;
       noff = 0;
    }

   if (fattr && (*fattr & FA_READONLY))
      mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);

   if ((fd = open(uname, flags, mode)) < 0)
      goto end;

   if (ClipMachineMemory->argc > 3)
    {				/* nOffset - defined */
       if (lseek(fd, noff, SEEK_SET) < 0)
	  goto end;
    }

   n = write(fd, str, strlen(str));

 end:
   _clip_retnl(ClipMachineMemory, n);

   if (lcut)
      ftruncate(fd, noff + n);
   if (fd >= 0)
      close(fd);

   if (uname)
      free(uname);

   return 0;
}

#if 0
	/* Uri: moved to funcs/ftools.prg */

/*
 *      TEMPFILE([<cDirectory>], [<cExtension>], [<nFileAttr>])
 *	   --> cFileName
 */

int
clip_TEMPFILE(ClipMachine * ClipMachineMemory)	/* Creates a file for temporary use */
{
/*
	   0       FA_NORMAL
	   1       FA_READONLY         READ ONLY
	   2       FA_HIDDEN           HIDDEN (Hidden files)
	   4       FA_SYSTEM           SYSTEM (System files)
	   32      FA_ARCHIVE          ARCHIVE (Changed since last backup)
*/
   char     *dname = _clip_parc(ClipMachineMemory, 1);

   char     *uname = _get_unix_name(ClipMachineMemory, dname);

  /*char *ext = _clip_parc( ClipMachineMemory, 2 ); */
   int       fd = -1, fattr = _clip_parni(ClipMachineMemory, 3);

   char      buf[MAXPATHLEN], retname[MAXPATHLEN];

   char /**tmp=NULL,*/ *dosdelim = "\\";

   mode_t    mode = S_IRUSR | S_IRGRP | S_IROTH;

   printf("\ntempfile");
   if (uname == NULL)
    {
       getcwd(buf, MAXPATHLEN);
       strcpy(retname, _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE));
       strcat(retname, _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[*retname - 65]));
    }
   else
    {
       strcpy(buf, uname);
       if (dname[1] == ':')
	{
	   retname[0] = 0;
	}
       else
	{
	   strcpy(retname, _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE));
	   if (dname[0] != dosdelim[0])
	      strcat(retname, dosdelim);
	}
       strcat(retname, dname);
    }

   if (ClipMachineMemory->argc < 2)
    {
       int      *p = _clip_fetch_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR);

       fattr = p ? *p : FA_ARCHIVE;
    }
   if (!(fattr & FA_READONLY))
      mode |= S_IWUSR | S_IWGRP | S_IWOTH;

   strcat(buf, "/");
   if (fattr & FA_HIDDEN)
      strcat(buf, ".");
   strcat(buf, "XXXXXX");

   if ((fd = mkstemp(buf)) >= 0)
    {
       close(fd);
       chmod(buf, mode);
       if (retname[strlen(retname) - 1] != dosdelim[0])
	  strcat(retname, dosdelim);
       printf("\nbuf=%s,%s\n", buf, retname);
       strcat(retname, strrchr(buf, '/') + 1);
       retname[0] = toupper(retname[0]);
       _clip_retc(ClipMachineMemory, retname);
    }
   else
    {
       _clip_retc(ClipMachineMemory, "");
    }

   if (uname)
      free(uname);
   return 0;
}
#endif

/*
 * NUMDISKF() --> nTotalFloppy
 */
int
clip_NUMDISKF(ClipMachine * ClipMachineMemory)	/* Determines the number of installed disk drives */
{
   _clip_retni(ClipMachineMemory, 0);
   return 0;
}

/*
 * NUMDISKH() --> nNumber
 */
int
clip_NUMDISKH(ClipMachine * ClipMachineMemory)	/* Determines the number of hard disks */
{
   char      hstr[3];

   int       i = 0, n = 0;

   hstr[1] = ':';
   hstr[2] = 0;

   for (i = 'A'; i <= 'Z'; i++)
    {
       hstr[0] = i;
       if (_clip_fetch_item(ClipMachineMemory, _clip_hashstr(hstr)) == NULL)
	  continue;
       n++;
    }
   _clip_retni(ClipMachineMemory, n);
   return 0;
}

/*
 * NUMDISKL() --> nNumber
 *
 * NUMDISKL() returns the number of available drives or the value specified
 * by LASTDRIVE = in CONFIG.SYS.  In a NOVELL network, you can determine
 * which is the first network drive (LASTDRIVE +1).
 * Normally, the function return value is 5, if there is no
 * deviating input in CONFIG.SYS for LASTDRIVE =.
 */
int
clip_NUMDISKL(ClipMachine * ClipMachineMemory)	/* Determines the number of available logical drives */
{
  /*
   * ����� �������, ��� ���� C: -> ��� �������
   */
   _clip_retni(ClipMachineMemory, 0);
   return 0;
}

/*
 * �������� ��� ����� unix, ������������ ��� ����� dos
 */
CLIP_DLLEXPORT int
_dos_path(ClipMachine * ClipMachineMemory, const char *fn, char *p, int len)
{
   return 0;
}

/*
 * �������� ��� ����� unix, ������������ ��� ����� dos
 */
int
clip_DOSPATH(ClipMachine * ClipMachineMemory)
{
   char      hstr[3], buf[MAXPATHLEN];

   int       i = 0, len = 0;

   char     *dsk = NULL, *ch = NULL, *uname = _clip_parc(ClipMachineMemory, 1);

   if (uname == NULL)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");*/
       return 1;
    }
   hstr[1] = ':';
   hstr[2] = 0;
   buf[0] = 0;
   for (i = 'A'; i <= 'Z'; i++)
    {
       hstr[0] = i;
       if ((dsk = _clip_fetch_item(ClipMachineMemory, _clip_hashstr(hstr))) == NULL)
	  continue;
       len = strlen(dsk);
       if (!memcmp(dsk, uname, len) && ((uname[len] == '/' || uname[len] == 0) || (len == 1 && dsk[len - 1] == '/')))
	{			/* found */
	   strcpy(buf, hstr);
	   strcat(buf, "\\");
	   if (uname[len] == '/')
	      strcat(buf, uname + len + 1);
	   else if (len == 1)
	      strcat(buf, uname + len);
	   ch = buf;
	   while ((ch = strchr(ch, '/')) != NULL)
	      *ch = '\\';
	   break;
	}
    }
   _clip_retc(ClipMachineMemory, buf);
   return 0;
}

int
clip_FILEMOVE(ClipMachine * ClipMachineMemory)	/* Moves files to another directory */
{
/*
	0      NO_DISK_ERR            No errors
	   -2      ER_FILE_NOT_FOUND      File not found
	   -3      ER_PATH_NOT_FOUND      Access path not found
	   -5      ER_ACCESS_DENIED       Access denied (e.g., network)
	   -17     ER_DIFFERENT_DEVICE    Target file not on same drive
*/
   char     *src = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   char     *dst = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2));

   int       ret = 0;

   if (src == NULL || dst == NULL)
    {
       ret = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");*/
       goto end;
    }
   if (access(dst, F_OK) == 0)
    {				/* file exist */
       _clip_retni(ClipMachineMemory, ER_ACCESS_DENIED);
       goto end;
    }

   if (rename(src, dst) == 0)
    {
       _clip_retni(ClipMachineMemory, NO_DISK_ERR);
    }
   else if (errno == EXDEV)
    {
       _clip_retni(ClipMachineMemory, -17);
    }
   else
    {
       _check_error(ClipMachineMemory, src, 0);
    }

 end:
   if (src)
      free(src);
   if (dst)
      free(dst);
   return ret;
}

int
clip_FILEVALID(ClipMachine * ClipMachineMemory)	/* Tests whether a string has a valid file name */
{
   char     *dname = _clip_parc(ClipMachineMemory, 1);

   if (dname == NULL && strlen(dname) > 64)
      _clip_retl(ClipMachineMemory, 0);
   else
      _clip_retl(ClipMachineMemory, 1);
   return 0;
}

int
clip_TRUENAME(ClipMachine * ClipMachineMemory)	/* Standardizes the path designation */
{
   char     *dname = NULL, *dsk = NULL, buf[MAXPATHLEN], s[MAXPATHLEN];

   int       len = 0, i = 0, n = 0, np = 0;

   if (ClipMachineMemory->argc < 1)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf( ClipMachineMemory, __FILE__, __LINE__, "invalid argument" );*/
       return 1;
    }
   dname = _clip_parc(ClipMachineMemory, 1);
   if (strlen(dname) > 1 && dname[1] == ':')
    {
       dsk = dname;
       strcpy(s, dname);
    }
   else
    {
       dsk = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);
       if (*dname == '\\')
	{
	   s[0] = 0;
	}
       else
	{
	   char     *dir = _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[*dsk - 65]);

	   for (i = 0, n = 0; dir[i]; i++, n++)
	      s[n] = (dir[i] == '/' ? '\\' : dir[i]);
	   s[n] = 0;
	   if (dir[1] != 0)
	      strcat(s, "\\");
	}
       strcat(s, dname);
    }
   len = strlen(s);

   for (i = 0, n = 0; i < len; i++)
    {
       switch (s[i])
	{
	case '\\':
	   if (i > 0 && s[i - 1] == '\\')
	      continue;
	   switch (np)
	    {
	    case 1:
	       n--;
	       np = 0;
	       continue;
	    case 2:
	       for (n -= 4; n > 0; n--)
		{
		   if (buf[n - 1] == '\\')
		      break;
		}
	       if (n <= 0)
		  n = 1;
	       np = 0;
	       continue;
	    }
	   break;
	case '.':
	   if (np > 0 || buf[n - 1] == '\\')
	      np++;
	   break;
	default:
	   np = 0;
	}
       buf[n++] = s[i];
    }
   if (np == 1)
    {
       n -= 2;
    }
   else if (np == 2)
    {
       for (n -= 4; n > 0; n--)
	{
	   if (buf[n - 1] == '\\')
	      break;
	}
    }
   if (n <= 0)
      n = 1;
   if (n > 1 && buf[n - 1] == '\\')
      n--;

   buf[n++] = 0;

   if (n < 2 || buf[1] != ':')
    {
       memmove(buf + 2, buf, n);
       memcpy(buf, dsk, 2);
    }

   _clip_retc(ClipMachineMemory, buf);
   return 0;
}

/*
 * Calculates/computes/determines a checksum for a file
 */
int
clip_FILECHECK(ClipMachine * ClipMachineMemory)
{
   char     *uname = NULL;

   FILE     *f = NULL;

   int       c = 0, checksum = 0, fd = -1;

   if (ClipMachineMemory->argc < 1)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf( ClipMachineMemory, __FILE__, __LINE__, "invalid argument" );*/
       return 1;
    }
   if ((uname =
	_get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1))) == NULL
       || (fd = open(uname, O_RDONLY)) == -1 || !_set_lock(fd, F_RDLCK) || (f = fdopen(fd, "r")) == NULL)
    {
       _clip_retni(ClipMachineMemory, -1);
       goto end;
    }

#define ROTATE_RIGHT(c) if ((c) & 01) (c) = ((c) >>1) + 0x8000; else (c) >>= 1;
   while ((c = getc(f)) != EOF)
    {
       ROTATE_RIGHT(checksum);
       checksum += c;
       checksum &= 0xffff;
    }
#undef ROTATE_RIGHT

 end:
   _clip_retni(ClipMachineMemory, checksum);
   if (uname != NULL)
      free(uname);
   if (f != NULL)
      fclose(f);
   return 0;
}

int
clip_SETFDATI(ClipMachine * ClipMachineMemory)	/* Sets the date and time of a file */
{
   char     *uname = NULL;

   int       i = 0, ret = 0;

   struct stat st;

   struct tm t_m, t_a;

   struct utimbuf ut, *u = NULL;

   if (ClipMachineMemory->argc < 1)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf( ClipMachineMemory, __FILE__, __LINE__, "invalid argument" );*/
       return 1;
    }
   if ((uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1))) == NULL)
    {
       goto end;
    }

   if (ClipMachineMemory->argc > 1)
    {
       if (stat(uname, &st))
	  goto end;
       t_a = *localtime(&st.st_atime);
       t_m = *localtime(&st.st_mtime);
       for (i = 2; i <= 3; i++)
	{
	   char     *ch = NULL;

	   int       year = -1, mon = -1, mday = -1, wday = -1;

	   int       hr = -1, min = -1, sec = -1;

	   long      julian = 0;

	   switch (_clip_parinfo(ClipMachineMemory, i))
	    {
	    case 0:
	       continue;
	    case CHARACTER_type_of_ClipVarType:
	       ch = _clip_parc(ClipMachineMemory, i);
	       sscanf(ch, "%d:%d:%d", &hr, &min, &sec);
	       if (sec < 0)
		  sec = 0;
	       if (min < 0)
		  min = 0;
	       if (hr < 0 || hr > 23 || min > 59 || sec > 59)
		  goto end;
	       t_m.tm_hour = hr;
	       t_m.tm_min = min;
	       t_m.tm_sec = sec;
	       break;
	    case DATE_type_of_ClipVarType:
	       julian = _clip_pardc(ClipMachineMemory, i, &year, &mon, &mday, &wday);
	       t_m.tm_year = year - 1900;
	       t_m.tm_mon = mon - 1;
	       t_m.tm_mday = mday;
	       break;
	    default:
	       goto end;
	    }
	}
       ut.actime = mktime(&t_a);
       ut.modtime = mktime(&t_m);
       u = &ut;
    }

   ret = utime(uname, u) ? 0 : 1;

 end:
   _clip_retl(ClipMachineMemory, ret);

   if (uname != NULL)
      free(uname);
   return 0;
}

int
clip_FILEDELETE(ClipMachine * ClipMachineMemory)	/* Deletes file(s) by name and attribute */
{
/*
	   0         FA_NORMAL
	   1         FA_READONLY       Read-only
	   2         FA_HIDDEN         HIDDEN (Concealed files)
	   4         FA_SYSTEM         SYSTEM (System files)
	   8         FA_VOLUME         VOLUME (Name of floppy/hard disk)
	   32        FA_ARCHIVE        ARCHIVE (Changed since last backup)
*/
   char     *s = NULL, *uname = NULL;

   char      path[MAXPATHLEN];

   char      buf[MAXPATHLEN];

   int       ret = 0, fattr = FA_ARCHIVE, plen = 0;

   DIR      *dir = NULL;

   struct dirent *d;

   struct stat st;

   if (ClipMachineMemory->argc < 1)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf( ClipMachineMemory, __FILE__, __LINE__, "invalid argument" );*/
       return 1;
    }
   if ((uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1))) == NULL)
    {
       goto end;
    }

   if ((s = strrchr(uname, '/')) == NULL)
    {				/* current directory */
       if ((dir = opendir(".")) == NULL)
	  goto end;
       path[0] = '.';
       path[1] = '/';
       path[2] = 0;
       plen = 2;
    }
   else if (*(s + 1) == 0)
    {				/* file name/mask expected */
       goto end;
    }
   else
    {
       plen = ++s - uname;
       memcpy(path, uname, plen);
       path[plen] = 0;
       if ((dir = opendir(path)) == NULL)
	  goto end;
    }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      fattr = _clip_parni(ClipMachineMemory, 2);

   while ((d = readdir(dir)) != NULL)
    {

       if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
	  continue;
#ifdef _WIN32
       if (_clip_glob_match(d->d_name, s, 1) != strlen(d->d_name))
#else
       if (_clip_glob_match(d->d_name, s, ClipMachineMemory->flags & TRANSLATE_FLAG) != strlen(d->d_name))
#endif
	  continue;
       stat(d->d_name, &st);
      /*
         if (!(((fattr & FA_ARCHIVE) && S_ISREG(st.st_mode)) ||
         ((fattr & FA_READONLY) &&
         (st.st_mode & S_IRUSR) && !(st.st_mode & S_IWUSR)) || ((fattr & FA_HIDDEN) && d->d_name[0] == '.')))
         {
         continue;
         }
       */
       strncpy(buf, path, MAXPATHLEN);
       strncpy(buf + plen, d->d_name, MAXPATHLEN - plen);
       if (unlink(buf) == 0)	/* success */
	  ret = 1;
    }

 end:
   _clip_retl(ClipMachineMemory, ret);

   if (uname != NULL)
      free(uname);
   if (dir)
      closedir(dir);
   return 0;
}

#undef FA_NORMAL
#undef FA_READONLY
#undef FA_HIDDEN
#undef FA_SYSTEM
#undef FA_VOLUME
#undef FA_DIRECTORY
#undef FA_ARCHIVE

/*
 * Sets default opening mode for CA-Clipper Tools file functions
 * ----------------------------------------------------------------------
 * Code    Share Mode
 * ----------------------------------------------------------------------
 * 0       Compatibility mode.  Here DOS or the network software itself
 *         determines the share mode.  In Novell networks, the SHAREABLE
 *         attribute plays an important role in this relationship.
 * 1       Both read and write by other programs are locked out.
 * 2       Write by other programs are locked out.
 * 3       Read by other programs are locked out.
 * 4       Not locked.  Read and write by other programs is allowed
 * ----------------------------------------------------------------------
 */
int
clip_SETSHARE(ClipMachine * ClipMachineMemory)
{
   int      *smode = NULL, n = 0;

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
    {
       _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
       return 1;
    }

   switch ((n = _clip_parni(ClipMachineMemory, 1)))
    {
    case LOCK_OS:
    case LOCK_RDWR:
    case LOCK_WRONLY:
    case LOCK_RDONLY:
    case LOCK_NONE:
       break;
    default:
       _clip_retl(ClipMachineMemory, 0);
       return 0;
    }

   smode = (int *) calloc(1, sizeof(int));

   *smode = n;
   _clip_store_item(ClipMachineMemory, CLIP_CA_SHARE_MODE, smode);
   _clip_retl(ClipMachineMemory, 1);
   return 0;
}

int
clip_GETSHARE(ClipMachine * ClipMachineMemory)	/* Determines the file open (share) mode */
{
   int      *smode = _clip_fetch_item(ClipMachineMemory, CLIP_CA_SHARE_MODE);

   _clip_retni(ClipMachineMemory, smode ? *smode : LOCK_RDONLY);
   return 0;
}

/*
  DISKCHECK() Creates a checksum for a disk
  DISKFORMAT() Formats disks, controlled through a UDF
  DISKSPEED() Determines a comparison value for the drive speed
  DISKSTAT() Determines the status of a drive.

  FILECCLOSE() Closes a file after backup mode
  FILECCONT() Copies sections of a file in backup mode
  FILECDATI() Determines which date the target file contains with FILECOPY()
  FILECOPEN() Tests to see if the file is still open in the backup mode
  FILECOPY() Copies files normally or in backup mode
  FLOPPYTYPE() Determines the exact type of floppy drive
  RENAMEFILE() Fault tolerant renaming of a file.
  VOLSERIAL() Determines the DOS disk serial number
  VOLUME() Establishes a volume label for a floppy or hard disk
 */

int
clip_FACCESS(ClipMachine * ClipMachineMemory)
{
   int       ret = 0, mode = 0;

   char     *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   char     *m = _clip_parc(ClipMachineMemory, 2);

   if (uname != NULL && m != NULL)
    {
       while (*m)
	{
	   switch (*m++)
	    {
	    case 'r':
	       mode |= R_OK;
	       break;
	    case 'w':
	       mode |= W_OK;
	       break;
	    case 'x':
	       mode |= X_OK;
	       break;
	    case 'f':
	       mode |= F_OK;
	       break;
	    }
	}
       ret = access(uname, mode) ? 0 : 1;
    }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
