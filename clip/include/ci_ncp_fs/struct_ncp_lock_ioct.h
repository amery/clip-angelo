struct ncp_lock_ioctl
{
#define NCP_LOCK_LOG	0
#define NCP_LOCK_SH	1
#define NCP_LOCK_EX	2
#define NCP_LOCK_CLEAR	256
int cmd_of_ncp_lock_ioctl;
int origin_of_ncp_lock_ioctl;
unsigned int offset_of_ncp_lock_ioctl;
unsigned int length_of_ncp_lock_ioctl;
#define NCP_LOCK_DEFAULT_TIMEOUT	18
#define NCP_LOCK_MAX_TIMEOUT		180
int timeout_of_ncp_lock_ioctl;
};
