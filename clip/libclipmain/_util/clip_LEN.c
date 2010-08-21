int
clip_LEN(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   int rn = 0, l, d;

   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
   {
      _clip_retndp(ClipMachineMemory, 0, 10, 0);
      return 0;
   }

   vp = _clip_par(ClipMachineMemory, 1);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      rn = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      break;
   case ARRAY_type_of_ClipVarType:
      rn = vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      break;
   case MAP_type_of_ClipVarType:
      rn = vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
      break;
   case NUMERIC_type_of_ClipVarType:
      rn = vp->ClipType_t_of_ClipVar.len_of_ClipType;
      if (_clip_parinfo(ClipMachineMemory, 2) == NUMERIC_type_of_ClipVarType)
	 vp->ClipType_t_of_ClipVar.len_of_ClipType = _clip_parni(ClipMachineMemory, 2);
      break;
   default:
      ;
   }

   l = 10;
   d = 0;
   if (ClipMachineMemory->flags & FIXED_FLAG)
   {
      d = ClipMachineMemory->decimals;
      l += ClipMachineMemory->decimals + 1;
   }
   _clip_retndp(ClipMachineMemory, rn, l, d);
   return 0;
}
