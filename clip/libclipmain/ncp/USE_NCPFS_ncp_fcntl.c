int
ncp_fcntl(int fd, int flag, void *argp)
{
   struct ncp_lock_ioctl ncp_lock_ioctl;

   struct flock *fl;

   int r, ret;

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
		ncp_lock_ioctl.cmd_of_ncp_lock_ioctl = NCP_LOCK_EX;
      break;
   case F_RDLCK:
		ncp_lock_ioctl.cmd_of_ncp_lock_ioctl = NCP_LOCK_SH;
      break;
   case F_UNLCK:
		ncp_lock_ioctl.cmd_of_ncp_lock_ioctl = NCP_LOCK_CLEAR;
      break;
   }

   ncp_lock_ioctl.origin_of_ncp_lock_ioctl = 0;
	ncp_lock_ioctl.offset_of_ncp_lock_ioctl = fl->l_start;
	ncp_lock_ioctl.length_of_ncp_lock_ioctl = fl->l_len;
	ncp_lock_ioctl.timeout_of_ncp_lock_ioctl = NCP_LOCK_DEFAULT_TIMEOUT;

   /* and do ncpfs-specific ioctl */

   for (;;)
   {
      r = ioctl(fd, NCP_IOC_LOCKUNLOCK, &ncp_lock_ioctl);
#if 0
      printf("\nncp ioctl: cmd %d, off 0x%x, len 0x%x, r %d, errno %d", ncp_lock_ioctl.cmd, ncp_lock_ioctl.offset, ncp_lock_ioctl.length, r, errno);
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
