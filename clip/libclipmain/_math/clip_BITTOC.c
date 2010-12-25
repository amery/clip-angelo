int
clip_BITTOC(ClipMachine * ClipMachineMemory)
{
   unsigned int Num = _clip_parni(ClipMachineMemory, 1);

   int LenPattern;

   char *BitPattern = _clip_parcl(ClipMachineMemory, 2, &LenPattern);

   char *p, res[] = "                ";

   unsigned int Mask;

   int Len = _clip_parinfo(ClipMachineMemory, 0), RealLen;

   int t1 = _clip_parinfo(ClipMachineMemory, 1);

   int t2 = _clip_parinfo(ClipMachineMemory, 2);

   if (Len < 2 || t1 != NUMERIC_type_of_ClipVarType || t2 != CHARACTER_type_of_ClipVarType)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "BITTOC");
   }

   if (LenPattern > 16)
      LenPattern = 16;
   if (_clip_parl(ClipMachineMemory, 3))
   {
      for (RealLen = LenPattern, Len = 0, Mask = 1 << (LenPattern - 1); Len < LenPattern; Mask >>= 1, Len++)
	 if (Num & Mask)
	    res[Len] = BitPattern[Len];
   }
   else
   {
      for (RealLen = 0, p = res, Len = 0, Mask = 1 << (LenPattern - 1); Len < LenPattern; Mask >>= 1, Len++)
	 if (Num & Mask)
	 {
	    *p = BitPattern[Len];
	    p++;
	    RealLen++;
	 }
   }
   _clip_retcn(ClipMachineMemory, res, RealLen);
   return 0;
}
