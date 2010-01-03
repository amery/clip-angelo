struct ncp_lock_ioctl
{
#define NCP_LOCK_LOG	0
#define NCP_LOCK_SH	1
#define NCP_LOCK_EX	2
#define NCP_LOCK_CLEAR	256
   int       cmd;
   int       origin;
   unsigned int offset;
   unsigned int length;
#define NCP_LOCK_DEFAULT_TIMEOUT	18
#define NCP_LOCK_MAX_TIMEOUT		180
   int       timeout;
};

