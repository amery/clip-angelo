int
_set_lock(int fd, int l_type)
{
   struct flock fl;

   int r;

   fl.l_type = l_type;
   fl.l_whence = SEEK_SET;
   fl.l_start = 0x7fffffff;
   fl.l_len = 1;
#ifndef OS_MINGW
   fl.l_pid = 0;
#endif
   r = fcntl(fd, F_SETLK, &fl);

#ifdef USE_NCPFS
   if (r)
      return !r;
   r = ncp_openmode(fd, l_type == F_WRLCK ? 1 : 0);
#endif

   return !r;
}
