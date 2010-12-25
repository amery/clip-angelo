int
ncp_openmode(int fd, int exclusive)
{
   int r;

   if (!ncp_is_ncpfs(fd))
      return 0;

   r = ioctl(fd, NCP_IOC_OPENMODE, &exclusive);

   /* invalid ioctl for this kernel */
   if (r == EINVAL)
      return 0;

   return r;
}
