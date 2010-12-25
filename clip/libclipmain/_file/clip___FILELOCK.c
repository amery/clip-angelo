int
clip___FILELOCK(ClipMachine * ClipMachineMemory)
{
   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   _clip_retl(ClipMachineMemory, 0);

   *err = 0;
   if (cf == NULL)
   {
      *err = EBADF;
      return 0;
   }

   if (_clip_setlock(ClipMachineMemory, cf->hash_of_C_FILE, cf->fileno_of_C_FILE, 1000000000 + 1, CLIP_LOCK_WRITE | CLIP_LOCK_FLOCK | CLIP_LOCK_HILEVEL))
      *err = ENOLCK;
   else
      _clip_retl(ClipMachineMemory, 1);
   return 0;
}
