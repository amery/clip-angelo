CLIP_DLLEXPORT void
_clip_push_nil(ClipMachine * ClipMachineMemory)
{
   ClipFrame *fp = ClipMachineMemory->fp;

   fp->ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
   fp->ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   fp->ClipVar_sp_of_ClipFrame++;
}
