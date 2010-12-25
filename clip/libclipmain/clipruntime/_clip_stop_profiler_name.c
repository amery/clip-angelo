int CLIP_DLLEXPORT
_clip_stop_profiler_name(ClipMachine * ClipMachineMemory, const char *procname, const char *filename)
{
   struct timeval tv;

   ProfileBucket *bp;

   if (!_clip_profiler)
      return 0;

   gettimeofday(&tv, 0);
   bp = find_profile_bucket_name(ClipMachineMemory, procname, filename);

   return stop_profiler(ClipMachineMemory, bp, &tv);
}
