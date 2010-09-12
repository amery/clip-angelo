TASK_DLLEXPORT int
clip_task_select_if(int fd, void *rp, void *wp, void *ep, void *to)
{
#ifdef _LDEBUG
   printf("F=%s,L=%d\n", __FILE__, __LINE__);
#endif
   if (!pth_inited)
      return -1;
   return task_select(fd, (fd_set *) rp, (fd_set *) wp, (fd_set *) ep, (struct timeval *) to);
}
