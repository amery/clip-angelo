int
clip_MEMVARLIST(ClipMachine * ClipMachineMemory)
{
   ClipVar *rp = RETPTR(ClipMachineMemory);

   char *pattern = _clip_parc(ClipMachineMemory, 1);

   int publ = _clip_parl(ClipMachineMemory, 2);

   int isLike = _clip_parl(ClipMachineMemory, 3);

   int all = _clip_parinfo(ClipMachineMemory, 2) != LOGICAL_type_of_ClipVarType;

   ClipFrame *fp;

   Coll snames;

   int r, i;

   int isPrivate, isPublic;
   long dims[] = { 0 };
   ClipVar vp;

   if (!pattern)
      pattern = "*";

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

	       if ((m && !isLike) || (!m && isLike))
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

	 if ((m && !isLike) || (!m && isLike))
	    atInsert_Coll(&snames, nm, ind);
      }
   }

   _clip_array(ClipMachineMemory, rp, 1, dims);

	for (i = 0; i < snames.count_of_Coll; i++)
   {
		long hash = _clip_casehashstr(snames.items_of_Coll[i]);

      isPublic = !_clip_is_public(ClipMachineMemory, hash);
      isPrivate = !_clip_is_private(ClipMachineMemory, hash);

      if (all)
	 r = isPublic || isPrivate;
      else if (publ)
	 r = isPublic;
      else
	 r = isPrivate;
      if (r)
      {
			_clip_var_str(snames.items_of_Coll[i], strlen(snames.items_of_Coll[i]), &vp);
	 _clip_aadd(ClipMachineMemory, rp, &vp);
	 _clip_destroy(ClipMachineMemory, &vp);
      }
   }
   destroy_Coll(&snames);
   return 0;
}
