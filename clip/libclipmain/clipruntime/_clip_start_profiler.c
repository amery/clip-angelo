int CLIP_DLLEXPORT
_clip_start_profiler(ClipMachine * ClipMachineMemory)
{
   ProfileBucket *bp;

   bp = find_profile_bucket(ClipMachineMemory);

   return start_profiler(ClipMachineMemory, bp);
}
