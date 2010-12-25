int
clip_NDATE(ClipMachine * ClipMachineMemory)
{
   int dd = _clip_parni(ClipMachineMemory, 1);

   int mm = _clip_parni(ClipMachineMemory, 2);

   int yy = _clip_parni(ClipMachineMemory, 3);

   ClipVar *rp = RETPTR(ClipMachineMemory);

   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _clip_jdate(dd, mm, yy);

   return 0;
}
