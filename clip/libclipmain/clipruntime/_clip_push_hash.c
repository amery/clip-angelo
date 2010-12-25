CLIP_DLLEXPORT void
_clip_push_hash(ClipMachine * ClipMachineMemory, long hash)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   CLEAR_CLIPVAR(sp);
   sp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   sp->ClipType_t_of_ClipVar.len_of_ClipType = 10;
   sp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   sp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = hash;

   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;
}
