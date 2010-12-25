static int
stop_profiler(ClipMachine * ClipMachineMemory, ProfileBucket * bp, struct timeval *tv)
{
   struct timeval dt, rt;

   if (!bp->started_of_ProfileBucket)
      return 0;

   timer_sub(tv, &bp->timeval_start_of_ProfileBucket, &dt);

   timer_add(&bp->timeval_self_of_ProfileBucket, &dt, &rt);
   bp->timeval_self_of_ProfileBucket = rt;
   bp->started_of_ProfileBucket = 0;

   return 0;
}
