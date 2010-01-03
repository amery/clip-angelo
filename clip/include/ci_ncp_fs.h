/*
 *  ncp_fs.h
 *
 *  Copyright (C) 1995, 1996 by Volker Lendecke
 *
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */

/* TODO: These are all internal to ncpfs... Do something with them :-) */

/* NOTE: this is a standalone version of linux/ncp_fs.h */

#ifndef CN__KERNEL_NCP_FS_H
#define CN__KERNEL_NCP_FS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ci_ncp_fs/struct_ncp_sign_init.h"
#include "ci_ncp_fs/union_ncp_sockaddr.h"

/*
 * ioctl commands
 */
#include "ci_ncp_fs/struct_ncp_ioctl_request.h"
#include "ci_ncp_fs/struct_ncp_fs_info.h"
#include "ci_ncp_fs/struct_ncp_lock_ioct.h"
#include "ci_ncp_fs/struct_ncp_setroot_ioctl.h"
#include "ci_ncp_fs/struct_ncp_objectname_ioctl.h"
#include "ci_ncp_fs/struct_ncp_privatedata_ioctl.h"




/* NLS charsets by ioctl */
#define NCP_IOCSNAME_LEN 20
#include "ci_ncp_fs/struct_ncp_nls_ioctl_old.h"
#include "ci_ncp_fs/struct_ncp_nls_ioctl.h"


#define	NCP_IOC_NCPREQUEST		_IOR('n', 1, struct ncp_ioctl_request)
#define	NCP_IOC_GETMOUNTUID		_IOW('n', 2, __kernel_uid_t)
#define NCP_IOC_CONN_LOGGED_IN          _IO('n', 3)

#define NCP_GET_FS_INFO_VERSION (1)
#define NCP_IOC_GET_FS_INFO             _IOWR('n', 4, struct ncp_fs_info)

#define NCP_IOC_SIGN_INIT		_IOR('n', 5, struct ncp_sign_init)
#define NCP_IOC_SIGN_WANTED		_IOR('n', 6, int)
#define NCP_IOC_SET_SIGN_WANTED		_IOW('n', 6, int)

#define NCP_IOC_LOCKUNLOCK		_IOR('n', 7, struct ncp_lock_ioctl)

#define NCP_IOC_GETROOT			_IOW('n', 8, struct ncp_setroot_ioctl)
#define NCP_IOC_SETROOT			_IOR('n', 8, struct ncp_setroot_ioctl)

#define NCP_IOC_GETOBJECTNAME		_IOWR('n', 9, struct ncp_objectname_ioctl)
#define NCP_IOC_SETOBJECTNAME		_IOR('n', 9, struct ncp_objectname_ioctl)
#define NCP_IOC_GETPRIVATEDATA		_IOWR('n', 10, struct ncp_privatedata_ioctl)
#define NCP_IOC_SETPRIVATEDATA		_IOR('n', 10, struct ncp_privatedata_ioctl)

#define NCP_IOC_GETCHARSETS		_IOWR('n', 11, struct ncp_nls_ioctl)
#define NCP_IOC_SETCHARSETS		_IOR('n', 11, struct ncp_nls_ioctl)
/* never released to Linus, will wanish shortly */
#define NCP_IOC_GETCHARSETS_OLD		_IOWR('n', 11, struct ncp_nls_ioctl_old)
#define NCP_IOC_SETCHARSETS_OLD		_IOR('n', 11, struct ncp_nls_ioctl_old)

#define NCP_IOC_GETDENTRYTTL		_IOW('n', 12, u_int32_t)
#define NCP_IOC_SETDENTRYTTL		_IOR('n', 12, u_int32_t)

/*
 * This ioctl allow work in dos-specific share/exclusive open mode
 * without using lockfiles and so on, and in compatible with dos
 * applications way.
 *
 * if ioctl *arg == 0, request for share open
 * else request for exclusive open
 * ioctl returns EINVAL if this feature is absent (invalid ioctl),
 * 0 on success, and other value on share errors.
 *
 * Paul Lasarev <paul@itk.ru>
 */
#define NCP_IOC_OPENMODE		_IOW('n', 21, int)

/*
#define N_AR_DENY_READ      (ntohs(0x0400))
#define N_AR_DENY_WRITE     (ntohs(0x0800))
int ncp_openmode(struct inode *inode, struct file *file, int exclusive);
*/

/*
 * The packet size to allocate. One page should be enough.
 */
#define NCP_PACKET_SIZE 4070

#define NCP_MAXPATHLEN 255
#define NCP_MAXNAMELEN 14

#endif /* _LINUX_NCP_FS_H */
