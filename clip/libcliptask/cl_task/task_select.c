TASK_DLLEXPORT int
task_select(int nfds, fd_set * readfds, fd_set * writefds, fd_set * exceptfds, struct timeval *timeout)
{
   struct timeval tv;

   int ret, r;

   Task *tp;

   if (!canSwitch)
   {
      ret = t_select(nfds, readfds, writefds, exceptfds, timeout);
      return ret;
   }

   tp = currTask;

   if (readfds)
   {
      tp->isRead = 1;
      tp->rfileset = *readfds;
   }
   else
      FD_ZERO(&tp->rfileset);

   if (writefds)
   {
      tp->isWrite = 1;
      tp->wfileset = *writefds;
   }
   else
      FD_ZERO(&tp->wfileset);

   if (exceptfds)
   {
      tp->isExcept = 1;
      tp->efileset = *exceptfds;
   }
   else
      FD_ZERO(&tp->efileset);

   tv.tv_sec = tv.tv_usec = 0;

   if (readfds || writefds || exceptfds)
   {
      ret = t_select(FD_SETSIZE, readfds, writefds, exceptfds, &tv);

      r = (ret != 0 || (timeout && !timeout->tv_sec && !timeout->tv_usec));
      if (r)
      {
	 Task_yield();
	 tp->isRead = 0;
	 tp->isWrite = 0;
	 tp->isExcept = 0;
	 return ret;
      }
   }

   tp->wakeUp = calc_wakeup(timeout);
   removeFromList(tp);
   addToWait(tp);
   tp->isTimed = 0;

   Task_yield();

   tp->isRead = 0;
   tp->isWrite = 0;
   tp->isExcept = 0;

   if (readfds)
      *readfds = tp->rfileset;
   if (writefds)
      *writefds = tp->wfileset;
   if (exceptfds)
      *exceptfds = tp->efileset;
   ret = !tp->isTimed;

   return ret;
}
