int CLIP_DLLEXPORT
_clip_start_profiler_name(ClipMachine * ClipMachineMemory, const char *procname, const char *filename)
{
   ProfileBucket *bp;

   if (!_clip_profiler)
      return 0;

   bp = find_profile_bucket_name(ClipMachineMemory, procname, filename);

   return start_profiler(ClipMachineMemory, bp);
}
