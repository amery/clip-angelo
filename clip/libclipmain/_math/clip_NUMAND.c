int
clip_NUMAND(ClipMachine * ClipMachineMemory)
{
   int Num;

   int i = _clip_parinfo(ClipMachineMemory, 0);

   int SLen;

   char *S;

   int tmp;

   if (i < 2)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMAND");
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
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMAND");
   }
   for (; i > 1; i--)
   {
      switch (_clip_parinfo(ClipMachineMemory, i))
      {
      case NUMERIC_type_of_ClipVarType:
	 tmp = _clip_parni(ClipMachineMemory, i);
	 break;
      case CHARACTER_type_of_ClipVarType:
	 S = _clip_parcl(ClipMachineMemory, i, &SLen);
	 StrToInt(S, SLen, &tmp);
	 break;
      default:
	 _clip_retni(ClipMachineMemory, 0);
	 return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMAND");
      }
      Num &= tmp;
   }
   _clip_retni(ClipMachineMemory, Num);
   return 0;
}
