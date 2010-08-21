TASK_DLLEXPORT int
Task_yield(void)
{
   Task *ct;

   int r;

   if (!canSwitch)
      return 0;

   ct = currTask;
   if (!ct)
      return 0;

   if ((r = setjmp(ct->taskEnv)) == 0)
      longjmp(shedEnv, 1);

   if (r == 2)
      return -1;

   return 0;
}
