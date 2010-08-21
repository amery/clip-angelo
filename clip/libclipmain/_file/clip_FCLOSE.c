int
clip_FCLOSE(ClipMachine * ClipMachineMemory)
{
   int fd = _clip_parni(ClipMachineMemory, 1);

   int ret = -1, *err;

   C_FILE *cf;

   _clip_retl(ClipMachineMemory, 0);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      return 0;

   cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);
   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (cf == NULL)
   {
      *err = EBADF;
   }
   else if (_clip_destroy_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE))
   {
      *err = 0;
      ret = 0;
   }

   _clip_retl(ClipMachineMemory, (ret == 0));
   return 0;
}
