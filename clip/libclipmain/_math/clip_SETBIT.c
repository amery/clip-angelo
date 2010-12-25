int
clip_SETBIT(ClipMachine * ClipMachineMemory)
{
   int Num, SLen;

   int i = _clip_parinfo(ClipMachineMemory, 0), Bit;

   char *S;

   if ((i == 1) || (i > 33))
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SETBIT");
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
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SETBIT");
   }
   for (; i > 1; i--)
   {
      Bit = _clip_parni(ClipMachineMemory, i);
      if (Bit > 0 && Bit <= 32)
	 Num |= 1 << (Bit - 1);
   }
   _clip_retni(ClipMachineMemory, Num);
   return 0;
}
