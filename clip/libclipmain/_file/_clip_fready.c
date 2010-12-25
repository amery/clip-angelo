int
_clip_fready(ClipMachine * ClipMachineMemory, int rmode)
{
   int ret = -1, *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   fd_set set;

   struct timeval tv;

   int fd = _clip_parni(ClipMachineMemory, 1), timeout;

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   _clip_retl(ClipMachineMemory, 0);

   if (cf == NULL)
   {
      *err = EBADF;
      return 0;
   }

   if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
      timeout = _clip_parni(ClipMachineMemory, 2);
   else
      timeout = cf->timeout_of_C_FILE;

   if (timeout < 0)
      timeout = 0;		/* TODO?: maybe we should set in such case
				   infinit timeout (&tv=NULL)? */

   fd = cf->fileno_of_C_FILE;

   FD_ZERO(&set);
   FD_SET(fd, &set);

   tv.tv_sec = timeout / 1000;
   tv.tv_usec = (timeout % 1000) * 1000;

   switch (rmode)
   {
   case 1:
      ret = _clip_select(fd + 1, &set, NULL, NULL, &tv);
      break;
   case 2:
      ret = _clip_select(fd + 1, NULL, &set, NULL, &tv);
      break;
   }

   *err = ret == -1 ? errno : 0;
   if (ret > 0)
      _clip_retl(ClipMachineMemory, 1);

   return 0;
}
