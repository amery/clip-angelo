int
clip_COMPLEMENT(ClipMachine * ClipMachineMemory)
{
   int len, dec;

   long d1, d2;

   int t = _clip_parinfo(ClipMachineMemory, 1);

   switch (t)
   {
   case LOGICAL_type_of_ClipVarType:
      _clip_retl(ClipMachineMemory, _clip_parl(ClipMachineMemory, 1));
      break;
   case NUMERIC_type_of_ClipVarType:
      _clip_parp(ClipMachineMemory, 1, &len, &dec);
      _clip_retndp(ClipMachineMemory, 0.00 - _clip_parnd(ClipMachineMemory, 1), len, dec);
      break;
   case CHARACTER_type_of_ClipVarType:
      clip_CHARNOT(ClipMachineMemory);
      break;
   case DATE_type_of_ClipVarType:
      d1 = _clip_jdate(1, 1, 3000);
      d2 = _clip_pardj(ClipMachineMemory, 1);
      if (d2 == 0)
	 _clip_retdj(ClipMachineMemory, d1);
      else
	 _clip_retdj(ClipMachineMemory, d1 - d2 + _clip_jdate(7, 1, 0));
      break;
   }
   return 0;
}
