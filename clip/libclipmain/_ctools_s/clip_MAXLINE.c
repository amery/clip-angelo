int
clip_MAXLINE(ClipMachine * ClipMachineMemory)
{
   int l, ret = 0;

   unsigned char *e, *beg, *end;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == 0)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MAXLINE");
   }
   for (e = str, beg = str, end = str + l; e <= end; e++)
   {
      if (*e == '\r')
	 beg++;
      if (*e != '\n' && *e != 0)
	 continue;
      if (ret < (e - beg))
	 ret = e - beg;
      beg = e + 1;
   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
