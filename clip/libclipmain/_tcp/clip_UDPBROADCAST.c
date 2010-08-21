int
clip_UDPBROADCAST(ClipMachine * ClipMachineMemory)
{
   int fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror), ret = -1, arg;

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
   {
      arg = _clip_parl(ClipMachineMemory, 2);
      ret = setsockopt(cf->fileno_of_C_FILE, SOL_SOCKET, SO_BROADCAST, (void *) &arg, sizeof(arg));
      *err = ret == -1 ? errno : 0;
   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
