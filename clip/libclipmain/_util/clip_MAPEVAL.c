int
clip_MAPEVAL(ClipMachine * ClipMachineMemory)
{
   ClipVar *obj = _clip_par(ClipMachineMemory, 1);

   ClipVar *bp = _clip_par(ClipMachineMemory, 2);

   ClipVar *sobj;

   int ret;

   if (!obj || (obj->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != MAP_type_of_ClipVarType))
      return EG_ARG;

   if (!bp
       || (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType
	   && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType))
      return EG_ARG;

   sobj = ClipMachineMemory->obj;
   ClipMachineMemory->obj = obj;
   if (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      ret =
	 _clip_eval_macro(ClipMachineMemory, bp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			  bp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, RETPTR(ClipMachineMemory));
   else
      ret = _clip_eval(ClipMachineMemory, bp, ClipMachineMemory->argc - 2, ARGPTR(ClipMachineMemory, 3), RETPTR(ClipMachineMemory));
   ClipMachineMemory->obj = sobj;

   return ret;
}
