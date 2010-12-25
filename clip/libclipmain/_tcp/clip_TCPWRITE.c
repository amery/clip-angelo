int
clip_TCPWRITE(ClipMachine * ClipMachineMemory)
{
   int lenbuf = 0, ret = -1;

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   int fd = _clip_parni(ClipMachineMemory, 1);

   char *buf = _clip_parcl(ClipMachineMemory, 2, &lenbuf);

   long len = _clip_parnl(ClipMachineMemory, 3);

   int timeout = _clip_parni(ClipMachineMemory, 4), oldtout;

   C_FILE *cf = _clip_fetch_c_item(ClipMachineMemory, fd, _C_ITEM_TYPE_FILE);

   if (cf == NULL || cf->type_of_C_FILE != FT_SOCKET)
      *err = EBADF;
   else
   {
      if (_clip_parinfo(ClipMachineMemory, 3) != NUMERIC_type_of_ClipVarType || len > lenbuf)
	 len = lenbuf;

      oldtout = cf->timeout_of_C_FILE;
      if (_clip_parinfo(ClipMachineMemory, 4) == NUMERIC_type_of_ClipVarType)
	 cf->timeout_of_C_FILE = timeout;

      if (len > 0)
      {
	 if (cf->file_of_C_FILE == NULL || (cf->stat_of_C_FILE & FS_BUFFERED) == 0)
	    ret = _clip_write(cf, buf, len);
	 else
	    ret = fwrite(buf, 1, len, cf->file_of_C_FILE);
      }
      else
	 ret = 0;

      cf->timeout_of_C_FILE = oldtout;

      *err = ret < 0 ? errno : 0;
   }
   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
