static int
start_profiler(ClipMachine * ClipMachineMemory, ProfileBucket * bp)
{
   ProfileBucket *pbp;

   struct timeval tv;

   pbp = ClipMachineMemory->pbucket;

   if (ClipMachineMemory->pbucket && ClipMachineMemory->pbucket->started_of_ProfileBucket)
      _clip_stop_profiler(ClipMachineMemory);

   if (bp->started_of_ProfileBucket)
   {
      ClipMachineMemory->pbucket = bp;
      _clip_stop_profiler(ClipMachineMemory);
   }

   gettimeofday(&tv, 0);
   bp->timeval_start_of_ProfileBucket = tv;
   ClipMachineMemory->pbucket = bp;
   bp->started_of_ProfileBucket = 1;
   bp->callno_of_ProfileBucket++;

   return 0;
}
