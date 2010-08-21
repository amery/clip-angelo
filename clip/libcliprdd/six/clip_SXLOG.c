int
clip_SXLOG(ClipMachine * ClipMachineMemory)
{
   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   ClipMachineMemory->m6_error = 0;
   switch (_clip_parinfo(ClipMachineMemory, 1))
   {
   case LOGICAL_type_of_ClipVarType:
      _clip_retl(ClipMachineMemory, v->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
      break;
   case CHARACTER_type_of_ClipVarType:
      if (toupper(*v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 'T' || toupper(*v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf) == 'Y')
	 _clip_retl(ClipMachineMemory, 1);
      else
	 _clip_retl(ClipMachineMemory, 0);
      break;
   case NUMERIC_type_of_ClipVarType:
      _clip_retl(ClipMachineMemory, v->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar != 0);
      break;
   default:
      _clip_retl(ClipMachineMemory, 0);
      break;
   }
   return 0;
}
