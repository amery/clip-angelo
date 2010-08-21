int
clip_ISBIT(ClipMachine * ClipMachineMemory)
{
   int Bit, SLen;

   int Num;

   int res = 0;

   char *S;

   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
   {
      _clip_retl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ISBIT");
   }
   switch (_clip_parinfo(ClipMachineMemory, 1))
   {
   case NUMERIC_type_of_ClipVarType:
      Num = _clip_parni(ClipMachineMemory, 1);
      break;
   case CHARACTER_type_of_ClipVarType:
      S = _clip_parcl(ClipMachineMemory, 1, &SLen);
      StrToInt(S, SLen, &Num);
      break;
   default:
      _clip_retl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ISBIT");
   }
   if (_clip_parinfo(ClipMachineMemory, 0) == 1)
   {
      res = (Num) ? 1 : 0;
   }
   else
   {
      Bit = _clip_parni(ClipMachineMemory, 2) - 1;
      if (Bit > 31 || Bit < 0)
	 Bit = 0;
      res = (Num & (1 << Bit)) ? 1 : 0;
   }
   _clip_retl(ClipMachineMemory, res);
   return 0;
}
