static ProfileBucket *
find_profile_bucket(ClipMachine * ClipMachineMemory)
{
   const char *fs = "", *ps = "";

   if (ClipMachineMemory->fp && ClipMachineMemory->fp->procname_of_ClipFrame)
      ps = ClipMachineMemory->fp->procname_of_ClipFrame;
   if (ClipMachineMemory->fp && ClipMachineMemory->fp->filename_of_ClipFrame)
      fs = ClipMachineMemory->fp->filename_of_ClipFrame;

   return find_profile_bucket_name(ClipMachineMemory, ps, fs);
}
