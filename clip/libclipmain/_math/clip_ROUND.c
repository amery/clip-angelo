int
clip_ROUND(ClipMachine * ClipMachineMemory)
{
#if 1
   int len, dec;

   double d = _clip_parnd(ClipMachineMemory, 1);

   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   int de = _clip_parni(ClipMachineMemory, 2);

#ifdef ARCH_i386
   (*(long long *) &d)++;
#endif

   _clip_parp(ClipMachineMemory, 1, &len, &dec);
   dec = de;
   if (dec <= 0)
      dec = 0;
   if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
   {
      char *tmp = rational_toString(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, 10, dec, 0);

      _clip_retnr(ClipMachineMemory, rational_fromString(tmp), len, dec);
      free(tmp);
      return 0;
   }
   if (d == 0.0)
   {
      _clip_retndp(ClipMachineMemory, d, len, de);
      return 0;
   }
   if (de == 0)
   {
      if (d < 0.0)
	 d = ceil(d - 0.5);
      else
	 d = floor(d + 0.5);
      _clip_retndp(ClipMachineMemory, d, len, dec);
      return 0;
   }
   if (de < 0)
   {
      double tmp = pow(10, -de);

      if (d < 0.0)
	 d = ceil((d / tmp) - 0.5);
      else
	 d = floor((d / tmp) + 0.5);
      d *= tmp;
   }
   else
   {
      double tmp = pow(10, de);

      if (d < 0.0)
	 d = ceil((d * tmp) - 0.5);
      else
	 d = floor((d * tmp) + 0.5);
      d /= tmp;
   }
   _clip_retndp(ClipMachineMemory, d, len, dec);
   return 0;
#else
   int len, dec, len1;

   char buf[33], ch;

   double ret, d = _clip_parnd(ClipMachineMemory, 1);

   int de = _clip_parni(ClipMachineMemory, 2);

   _clip_parp(ClipMachineMemory, 1, &len, &dec);

   buf[32] = 0;
   if (len > 30)
      len = 30;
   _clip_dtostr(buf, len, de + 1, d, 0);
   len1 = strlen(buf) - 1;
   ch = buf[len1];
   if (ch >= '5')
      buf[len1 - 1]++;
   buf[len1] = 0;
   ret = _clip_strtod(buf, &dec);
   _clip_retndp(ClipMachineMemory, ret, len, de);
   return 0;
#endif
}
