/* cFilename, cPattern, bLike */
int
clip___MSAVE(ClipMachine * ClipMachineMemory)
{
   char *path = _clip_parc(ClipMachineMemory, 1);

   char *pattern = _clip_parc(ClipMachineMemory, 2);

   int isLike = _clip_parl(ClipMachineMemory, 3);

   char buf[256], errbuf[64];

   int r, i;

   DbfData *vars = 0;

   ClipFrame *fp;

   Coll snames;

   char *e, *s;

   if (!path || !pattern)
      return EG_ARG;

   init_Coll(&snames, 0, strcasecmp);

   for (fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
   {
      if (fp->names_of_ClipFrame)
      {
	 ClipHashBucket *bp;

	 for (i = 0; i < fp->names_of_ClipFrame->num_of_ClipHashNames; i++)
	 {
	    char *nm;

	    int ind;

	    bp = fp->names_of_ClipFrame->buckets_of_ClipHashNames + i;
	    nm = fp->names_of_ClipFrame->mem_of_ClipHashNames + bp->offs_of_ClipHashBucket;
	    if (!search_Coll(&snames, nm, &ind))
	    {
	       int m = (_clip_glob_match(nm, pattern, 1) == -1 ? 0 : 1);

	       if ((m && isLike) || (!m && !isLike))
		  atInsert_Coll(&snames, nm, ind);
	    }
	 }
      }
   }

   for (r = HashTable_first(ClipMachineMemory->hashnames); r; r = HashTable_next(ClipMachineMemory->hashnames))
   {
      char *nm = (char *) HashTable_current(ClipMachineMemory->hashnames);

      int ind;

      if (!search_Coll(&snames, nm, &ind))
      {
	 int m = (_clip_glob_match(nm, pattern, 1) == -1 ? 0 : 1);

	 if ((m && isLike) || (!m && !isLike))
	    atInsert_Coll(&snames, nm, ind);
      }
   }

   vars = NEWVECT(DbfData, snames.count_of_Coll);

	for (i = 0; i < snames.count_of_Coll; i++)
   {
      long hash;

      ClipVar *vp;

		hash = _clip_casehashstr(snames.items_of_Coll[i]);

      vp = _clip_ref_memvar(ClipMachineMemory, hash);
      clip2dbf(vp, vars + i);
   }

   _clip_path(ClipMachineMemory, path, buf, sizeof(buf), 1);

   e = strrchr(buf, '.');
   s = strrchr(buf, '/');
   if (!e || (s && e < s))
      strncat(buf, ".mem", sizeof(buf));

	r = write_MemFile(buf, dbf_get_locale(ClipMachineMemory), vars, (char **) snames.items_of_Coll, snames.count_of_Coll, errbuf, sizeof(errbuf));

	for (i = 0; i < snames.count_of_Coll; i++)
      destroy_DbfData(vars + i);

   destroy_Coll(&snames);
   free(vars);

   if (r)
   {
      _clip_trap_str(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, errbuf);
      return EG_ARG;
   }

   return 0;
}
