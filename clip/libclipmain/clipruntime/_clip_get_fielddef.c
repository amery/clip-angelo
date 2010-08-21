CLIP_DLLEXPORT ClipFieldDef *
_clip_get_fielddef(ClipMachine * ClipMachineMemory, long areahash, long namehash)
{
   long hash;

   ClipFieldDef *fp;

   hash = _clip_hashbytes(0, (char *) &areahash, sizeof(areahash));
   hash = _clip_hashbytes(hash, (char *) &namehash, sizeof(namehash));

   fp = (ClipFieldDef *) HashTable_fetch(ClipMachineMemory->fields, hash);
   if (!fp)
   {
      fp = (ClipFieldDef *) calloc(1, sizeof(ClipFieldDef));
      fp->areahash_of_ClipFieldDef = areahash;
      fp->fieldhash_of_ClipFieldDef = namehash;
      HashTable_insert(ClipMachineMemory->fields, fp, hash);
   }

   return fp;
}
