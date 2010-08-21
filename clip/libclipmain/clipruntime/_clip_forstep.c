CLIP_DLLEXPORT int
_clip_forstep(ClipMachine * ClipMachineMemory, int *_if)
{
   double d_step, d_to, d_val;

   ClipVar *step = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);

   ClipVar *to = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 2);

   ClipVar *val = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 3);

   if (step->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType
       || (to->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType
	   && to->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATE_type_of_ClipVarType)
       || (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType && val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != DATE_type_of_ClipVarType))
      return EG_ARG;

   d_step = _clip_double(step);
   d_to = _clip_double(to);
   d_val = _clip_double(val);

   *_if = 1;

   if (d_step > 0)
   {
      if (d_val > d_to)
	 *_if = 0;
   }
   else if (d_step < 0)
   {
      if (d_val < d_to)
	 *_if = 0;
   }

   _clip_pop(ClipMachineMemory);
   _clip_pop(ClipMachineMemory);
   _clip_pop(ClipMachineMemory);

   return 0;
}
