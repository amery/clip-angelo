int
clip_ISMEMVAR(ClipMachine * ClipMachineMemory)
{
   int l, ret = 0;

   char *name = _clip_parcl(ClipMachineMemory, 1, &l);

   if (name)
   {
      if (!_clip_is_memvar(ClipMachineMemory, _clip_casehashbytes(0, name, l)))
	 ret = 1;

      if (!ret)
      {
	 ret = _clip_parse_name(ClipMachineMemory, name, l, 0, 0, 0, 0, 0, 0);
	 if (ret == 1 || ret == 3)
	    ret = 1;
	 else
	    ret = 0;
      }
   }
   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
