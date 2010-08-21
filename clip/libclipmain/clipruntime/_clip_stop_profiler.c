int CLIP_DLLEXPORT
_clip_stop_profiler(ClipMachine * ClipMachineMemory)
{
   struct timeval tv;

   ProfileBucket *bp;

   gettimeofday(&tv, 0);
   bp = ClipMachineMemory->pbucket;
   if (!bp)
      bp = find_profile_bucket(ClipMachineMemory);

   return stop_profiler(ClipMachineMemory, bp, &tv);
}
