static void
fini_profiler(ClipMachine * ClipMachineMemory)
{
   Coll coll;

   int r, i;

   FILE *out;

   char path[256];

   struct timeval total;

   unsigned long ms, us;

   init_profiler(ClipMachineMemory);
   /* generate report here */

   init_Coll(&coll, 0, cmp_profile_out);

   snprintf(path, sizeof(path), "%s.pro", _clip_progname);
   out = fopen(path, "w");
   if (!out)
      out = stderr;

   for (r = HashTable_first(ClipMachineMemory->profiler); r; r = HashTable_next(ClipMachineMemory->profiler))
   {
      ProfileBucket *bp = (ProfileBucket *) HashTable_current(ClipMachineMemory->profiler);

      insert_Coll(&coll, bp);
   }

   total.tv_sec = 0;
   total.tv_usec = 0;
	for (i = 0; i < coll.count_of_Coll; i++)
   {
		ProfileBucket *bp = (ProfileBucket *) coll.items_of_Coll[i];

      struct timeval tv;

      if (!bp->procname_of_ProfileBucket[0] && !bp->filename_of_ProfileBucket[0])
	 continue;

      ms = bp->timeval_self_of_ProfileBucket.tv_sec * 1000 + bp->timeval_self_of_ProfileBucket.tv_usec / 1000;
      us = bp->timeval_self_of_ProfileBucket.tv_usec % 1000;

      tv = total;
      timer_add(&tv, &bp->timeval_self_of_ProfileBucket, &total);

      fprintf(out, "%18s %-13s %8ld calls %8lu.%03lu ms\n", bp->procname_of_ProfileBucket, bp->filename_of_ProfileBucket, bp->callno_of_ProfileBucket, ms, us);
   }

   ms = total.tv_sec * 1000 + total.tv_usec / 1000;
   us = total.tv_usec % 1000;
   fprintf(out, "---------------------------------\n");
   fprintf(out, "total registered %8lu.%03lu ms\n", ms, us);

   destroy_Coll(&coll);

   if (out != stderr)
      fclose(out);
}
