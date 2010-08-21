static int
_clip_sockclose(ClipMachine * ClipMachineMemory)
{
   int fd = _clip_parni(ClipMachineMemory, 1);

   C_FILE *cf = (C_FILE *) _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   int ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   fd_set rfds;

   struct timeval tv;

   int arg;

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
   {
      *err = EBADF;
   }
   arg = fcntl(cf->fileno_of_C_FILE, F_GETFL, 0);
   if (!(arg & O_NONBLOCK))
   {
      FD_ZERO(&rfds);
      FD_SET(cf->fileno_of_C_FILE, &rfds);
      tv.tv_sec = 1;		//timeout / 1000;
      tv.tv_usec = 100;		//(timeout % 1000) * 1000;
      select(cf->fileno_of_C_FILE + 1, &rfds, NULL, NULL, &tv);
   }
   if (_clip_destroy_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE))
   {
      *err = ret = 0;
   }
   else
   {
      *err = errno;
   }

   /* _clip_retni(ClipMachineMemory, ret); TODO? What type of value should return */
   _clip_retl(ClipMachineMemory, (ret == 0));
   return 0;
}
