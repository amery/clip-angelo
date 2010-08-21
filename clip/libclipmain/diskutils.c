/*
	Copyright (C) 2001  ITK
	Author   : Sergey Rosenfeld <serg@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <ci_clipcfg.h>
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

#include <ci_clip.h>
#include <ci_error.ch>

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

#include <ci_hashcode.h>

/*
 * ��� �������
 */
#define PR(ptr)	printf( "'%s'\n", ptr ? ptr : "null" )

/*
 * ��� ������ hash-������ ��� ������� ������� ��������� �� ����
 * ��������� ������
 */
long _hash_cur_dir[26];

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

#include <diskutils/clip_INIT__CTOOLS_DISKFUNC.c>
#include <diskutils/clip_INI__CTOOLS_DISKFUNC.c>
#include <diskutils/_get_path.c>
#include <diskutils/_check_error.c>
#include <diskutils/_get_unix_name.c>
#include <diskutils/_get_disk_path.c>

extern int _set_lock(int fd, mode_t mode);

/*extern int _check_lock( int fd, int l_type );*/

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

#include <diskutils/clip_DELETEFILE.c>
#include <diskutils/clip_DIRCHANGE.c>
#include <diskutils/clip_DIRMAKE.c>
#include <diskutils/clip_DIRNAME.c>
#include <diskutils/clip_DIRREMOVE.c>
#include <diskutils/clip_DISKCHANGE.c>
#include <diskutils/_clip_curdir.c>
#include <diskutils/clip_CURDIR.c>
#include <diskutils/clip_DISKFREE.c>
#include <diskutils/clip_DISKNAME.c>
#include <diskutils/clip_DISKREADY.c>
#include <diskutils/clip_DISKREADYW.c>
#include <diskutils/clip_DISKTOTAL.c>
#include <diskutils/clip_DISKTYPE.c>
#include <diskutils/clip_DRIVETYPE.c>
#include <diskutils/clip_FILEAPPEND.c>
#include <diskutils/clip___COPYFILE.c>

#include <diskutils/typedef_struct_FSdata.h>

#include <diskutils/destroy_FSdata.c>
#include <diskutils/clip_FILESEEK.c>
#include <diskutils/_get_fileseek_info.c>
#include <diskutils/clip_FILEATTR.c>
#include <diskutils/clip_FILEATTRS.c>
#include <diskutils/clip_FILEDATE.c>
#include <diskutils/clip_FILESIZE.c>
#include <diskutils/clip_FILETIME.c>
#include <diskutils/clip_SAVEFSEEK.c>
#include <diskutils/clip_RESTFSEEK.c>
#include <diskutils/clip_SETFATTR.c>
#include <diskutils/clip_FILESTR.c>
#include <diskutils/clip_SETFCREATE.c>
#include <diskutils/clip_STRFILE.c>
#include <diskutils/clip_NUMDISKF.c>
#include <diskutils/clip_NUMDISKH.c>
#include <diskutils/clip_NUMDISKL.c>
#include <diskutils/_dos_path.c>
#include <diskutils/clip_DOSPATH.c>
#include <diskutils/clip_FILEMOVE.c>
#include <diskutils/clip_FILEVALID.c>
#include <diskutils/clip_TRUENAME.c>
#include <diskutils/clip_FILECHECK.c>
#include <diskutils/clip_SETFDATI.c>
#include <diskutils/clip_FILEDELETE.c>

#undef FA_NORMAL
#undef FA_READONLY
#undef FA_HIDDEN
#undef FA_SYSTEM
#undef FA_VOLUME
#undef FA_DIRECTORY
#undef FA_ARCHIVE

#include <diskutils/clip_SETSHARE.c>
#include <diskutils/clip_GETSHARE.c>
#include <diskutils/clip_FACCESS.c>
