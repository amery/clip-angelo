TASK_DLLEXPORT int
Task_wait_write(int fd, long msec)
{
   Task *tp;

   struct timeval tv;

   int r;

   if (!canSwitch)
   {
      fd_set set;

      FD_ZERO(&set);
      FD_SET(fd, &set);
      calcTv(&tv, msec);
      r = t_select(FD_SETSIZE, 0, &set, 0, &tv);
      if (r > 0)
	 return 0;
      else if (r == 0)
	 return 1;
      else
	 return r;
   }

   tp = currTask;

   FD_ZERO(&tp->wfileset);
   FD_SET(fd, &tp->wfileset);
   tv.tv_sec = tv.tv_usec = 0;
   r = t_select(FD_SETSIZE, 0, &tp->wfileset, 0, &tv);

   if (r != 0)
   {
      Task_yield();
      if (r > 0)
	 return 0;
      else
	 return r;
   }

   FD_SET(fd, &tp->wfileset);

   tp->wakeUp = calcWakeup(msec);
   removeFromList(tp);
   tp->isWrite = 1;
   addToWait(tp);
   tp->isTimed = 0;

   Task_yield();

   tp->isWrite = 0;
   return tp->isTimed;
}
