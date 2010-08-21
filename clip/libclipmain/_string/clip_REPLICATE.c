int
clip_REPLICATE(ClipMachine * ClipMachineMemory)
{
   int i, k, j, vl, len;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   int kol = _clip_parni(ClipMachineMemory, 2);

   char *ret;

   if (kol <= 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }

   if (vp == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "REPLICATE");
   }
   len = vl * kol;
   ret = malloc(len + 1);
   for (i = 0, k = 0; i < kol; i++)
      for (j = 0; j < vl; j++, k++)
	 ret[k] = vp[j];
   ret[len] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, len);

   return 0;
}
