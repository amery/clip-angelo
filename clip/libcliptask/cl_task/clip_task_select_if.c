TASK_DLLEXPORT int
clip_task_select_if(int fd, void *rp, void *wp, void *ep, void *to)
{
   return task_select(fd, (fd_set *) rp, (fd_set *) wp, (fd_set *) ep, (struct timeval *) to);
}
