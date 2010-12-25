TASK_DLLEXPORT long
Task_sleep(long msec)
{
   Task *tp;

   if (!canSwitch)
   {
      struct timeval tv;

      calcTv(&tv, msec);
      select(0, 0, 0, 0, &tv);
      return 1;
   }
   tp = currTask;
   tp->wakeUp = calcWakeup(msec);
   removeFromList(tp);
   addToWait(tp);
   tp->isTimed = 1;
   Task_yield();
   return !tp->isTimed;
}
