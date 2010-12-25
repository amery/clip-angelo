int
clip_HARDCR(ClipMachine * ClipMachineMemory)
{
   int vl = 0;

   char *ret, *e;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "HARDCR");
   }
   ret = malloc(vl + 1);
   memcpy(ret, vp, vl);
   for (e = ret; e < ret + vl; e++)
      if (*e == (char) 141)
	 *e = '\n';
   ret[vl] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, vl);
   return 0;
}
