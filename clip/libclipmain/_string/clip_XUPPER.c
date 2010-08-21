int
clip_XUPPER(ClipMachine * ClipMachineMemory)
{
   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
   {
      int l;

      char *s;

      char *r;

      s = _clip_parcl(ClipMachineMemory, 1, &l);
      if (!s)
      {
	 _clip_retc(ClipMachineMemory, "");
	 return 0;
      }

      r = _clip_memdup(s, l);
      translate((unsigned char *) r, l, 1);

      _clip_retcn_m(ClipMachineMemory, r, l);
   }
   else
   {
      ClipVar *r = RETPTR(ClipMachineMemory);

      _clip_dup(ClipMachineMemory, r, _clip_par(ClipMachineMemory, 1));
   }

   return 0;
}
