int
clip_NUMXOR(ClipMachine * ClipMachineMemory)
{
   int Num1, Num2, SLen;

   char *S;

   if (_clip_parinfo(ClipMachineMemory, 0) != 2)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMXOR");
   }
   switch (_clip_parinfo(ClipMachineMemory, 1))
   {
   case NUMERIC_type_of_ClipVarType:
      Num1 = _clip_parni(ClipMachineMemory, 1);
      break;
   case CHARACTER_type_of_ClipVarType:
      S = _clip_parcl(ClipMachineMemory, 1, &SLen);
      StrToInt(S, SLen, &Num1);
      break;
   default:
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMXOR");
   }
   switch (_clip_parinfo(ClipMachineMemory, 2))
   {
   case NUMERIC_type_of_ClipVarType:
      Num2 = _clip_parni(ClipMachineMemory, 2);
      break;
   case CHARACTER_type_of_ClipVarType:
      S = _clip_parcl(ClipMachineMemory, 2, &SLen);
      StrToInt(S, SLen, &Num2);
      break;
   default:
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMXOR");
   }
   _clip_retni(ClipMachineMemory, Num1 ^ Num2);
   return 0;
}
