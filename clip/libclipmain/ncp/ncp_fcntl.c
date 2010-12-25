int
ncp_fcntl(int fd, int flag, void *argp)
{
   return fcntl(fd, flag, argp);
}
