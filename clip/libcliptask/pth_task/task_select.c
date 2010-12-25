TASK_DLLEXPORT int
task_select(int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, struct timeval *timeout)
{
   int ret = 0;

#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (!pth_inited)
      return -1;
   if (!canSwitch)
      ret = select(nfds, readfds, writefds, exceptfds, timeout);
   else
      ret = pth_select(nfds, readfds, writefds, exceptfds, timeout);
   return ret;
}
