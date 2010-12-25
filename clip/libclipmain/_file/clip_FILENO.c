int
clip_FILENO(ClipMachine * ClipMachineMemory)
{
   int fd = _clip_parni(ClipMachineMemory, 1), *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   if (cf == NULL)
   {
      *err = EBADF;
      _clip_retni(ClipMachineMemory, -1);
   }
   else
   {
      *err = 0;
      _clip_retni(ClipMachineMemory, cf->fileno_of_C_FILE);
   }
   return 0;
}
