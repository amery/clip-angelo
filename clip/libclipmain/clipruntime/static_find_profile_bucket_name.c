static ProfileBucket *
find_profile_bucket_name(ClipMachine * ClipMachineMemory, const char *procname, const char *filename)
{
   long hash;

   ProfileBucket *bp;

   hash = _clip_casehashbytes(0, procname, strlen(procname));
   hash = _clip_casehashbytes(hash, filename, strlen(filename));

   init_profiler(ClipMachineMemory);
   bp = (ProfileBucket *) HashTable_fetch(ClipMachineMemory->profiler, hash);
   if (!bp)
   {
      bp = NEW(ProfileBucket);
      bp->hash_of_ProfileBucket = hash;
      bp->procname_of_ProfileBucket = strdup(procname);
      bp->filename_of_ProfileBucket = strdup(filename);
#if 0
      init_Coll(&bp->callby, delete_profile_bucket, cmp_profile_bucket);
#endif
      HashTable_insert(ClipMachineMemory->profiler, bp, hash);
   }

   return bp;
}
