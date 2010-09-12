CLIP_DLLEXPORT int
_clip_take_field(ClipMachine * ClipMachineMemory, long hash, long area, ClipVar * dest)
{
   DBWorkArea *wa;

   const char nm[] = "_clip_take_field";

   int fno, er;

   if (!(wa = get_area(ClipMachineMemory, area, 0, 0)))
      return 1;

   if ((fno = _rdd_fieldno(wa->rd, hash)) == -1)
      return 1;

   dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
   dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;

   if ((er = _rdd_takevalue(ClipMachineMemory, wa, fno, hash, dest, nm)))
      return er;
   return 0;
}
