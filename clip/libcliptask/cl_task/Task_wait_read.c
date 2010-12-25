TASK_DLLEXPORT int
Task_wait_read(int fd, long msec)
{
   Task *tp;

   struct timeval tv;

   fd_set set;

   int r;

   if (!canSwitch)
   {

      FD_ZERO(&set);
      FD_SET(fd, &set);
      calcTv(&tv, msec);
      r = t_select(FD_SETSIZE, &set, 0, 0, &tv);
      if (r > 0)
	 return 0;
      else if (r == 0)
	 return 1;
      else
	 return r;
   }

   tp = currTask;

   FD_ZERO(&set);
   FD_SET(fd, &set);
   tv.tv_sec = tv.tv_usec = 0;
   r = t_select(FD_SETSIZE, &set, 0, 0, &tv);

   if (r != 0)
   {
      Task_yield();
      if (r > 0)
	 return 0;
      else
	 return r;
   }

   FD_ZERO(&tp->rfileset);
   FD_SET(fd, &tp->rfileset);

   tp->wakeUp = calcWakeup(msec);
   removeFromList(tp);
   tp->isRead = 1;
   addToWait(tp);
   tp->isTimed = 0;

   Task_yield();

   tp->isRead = 0;
   return tp->isTimed;
}
