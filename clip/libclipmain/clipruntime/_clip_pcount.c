
CLIP_DLLEXPORT void
_clip_pcount(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = ClipMachineMemory->argc;
   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame++;
   CLIP_CHECK_STACK;
}
