int
clip_ISPRIVATE(ClipMachine * ClipMachineMemory)
{
   int l, ret = 0;

   char *name = _clip_parcl(ClipMachineMemory, 1, &l);

   if (name)
   {
      if (!_clip_is_private(ClipMachineMemory, _clip_casehashbytes(0, name, l)))
	 ret = 1;
   }

   _clip_retl(ClipMachineMemory, ret);
   return 0;
}
