void
_clip_hash_buckets(ClipMachine * ClipMachineMemory, ClipFile * file)
{
   char *hp = 0;

   int len = 0;

   int count = 0;

   int i;

   int nstrings;

   int nstatics;

   ClipHashNames *hash_names;

   ClipHashBucket *buckets;

   char *modbeg;

   ClipVarDef *statics;

   if (file->hash_names_of_ClipFile)
      return;

   modbeg = file->body_of_ClipFile + 8 + 2 * sizeof(long);

   nstrings = GETSHORT(F_OFFS(modbeg, 2, 1, 0));
   get_str(modbeg, nstrings - 1, &hp, &len);

   count = GETLONG(F_OFFS(hp, 0, 0, 0));
   nstatics = GETLONG(F_OFFS(hp, 1, 0, 0));

   hash_names = NEW(ClipHashNames);
   if (count)
      buckets = (ClipHashBucket *) calloc(sizeof(ClipHashBucket), count);
   else
      buckets = 0;
   hash_names->num_of_ClipHashNames = count;
   hash_names->buckets_of_ClipHashNames = buckets;
   hash_names->mem_of_ClipHashNames = F_OFFS(hp, 2 + nstatics + 2 * count, 0, 0);

   for (i = 0; i < count; i++)
   {
      long hash, offs;

      hash = buckets[i].hash_of_ClipHashBucket = GETLONG(F_OFFS(hp, 2 + nstatics + 2 * i, 0, 0));
      offs = buckets[i].offs_of_ClipHashBucket = GETLONG(F_OFFS(hp, 2 + nstatics + 2 * i + 1, 0, 0));
      if (_clip_debuglevel)
      {
	 char *str, *s;

	 s = (char *) HashTable_fetch(ClipMachineMemory->hashnames, hash);
	 if (!s)
	 {
	    str = hash_names->mem_of_ClipHashNames + offs;
	    s = strdup(str);
	    HashTable_store(ClipMachineMemory->hashnames, s, hash);
	 }
      }
   }

   file->hash_names_of_ClipFile = hash_names;

   statics = file->ClipVarDef_staticDefs_of_ClipFile = (ClipVarDef *) calloc(sizeof(ClipVarDef), nstatics + 1);

   file->ClipVarDef_staticDefs_of_ClipFile[0].name_of_ClipVarDef = nstatics;
   for (i = 0; i < nstatics; i++)
   {
      statics[i + 1].name_of_ClipVarDef = GETLONG(F_OFFS(hp, 2 + i, 0, 0));
      statics[i + 1].ClipVar_vp_of_ClipVarDef = file->ClipVar_statics_of_ClipFile + i + 1;
   }
}
