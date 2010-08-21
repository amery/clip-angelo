int
clip_SLEEP(ClipMachine * ClipMachineMemory)
{
   double d = 60000;

   long ld;

#ifndef USE_TASKS
   struct timeval tv;
#endif

   if (ClipMachineMemory->argc > 0)
      d = _clip_parnd(ClipMachineMemory, 1);

   ld = d;

#ifdef USE_TASKS
   Task_sleep(d * 1000);
#else

   tv.tv_sec = ld;
   tv.tv_usec = (d - (double) ld) * 1000000.0;

   select(0, 0, 0, 0, &tv);
#endif
   return 0;
}
