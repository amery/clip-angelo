CLIP_DLLEXPORT int
_clip_try_field(ClipMachine * ClipMachineMemory, long hash)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   DBWorkArea *wa;

   const char nm[] = "_clip_try_field";

   int no;

   if (!(wa = cur_area(ClipMachineMemory)))
      return 1;

   if ((no = _rdd_fieldno(wa->rd, hash)) == -1)
      return 1;

   sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
   sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
   if (_rdd_takevalue(ClipMachineMemory, wa, no, hash, sp, nm))
      return 1;

   return 0;
}
