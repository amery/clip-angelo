CLIP_DLLEXPORT ClipVar *
_clip_pop_ref(ClipMachine * ClipMachineMemory)
{
   return ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
}
