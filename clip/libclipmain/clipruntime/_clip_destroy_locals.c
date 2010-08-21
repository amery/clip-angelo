CLIP_DLLEXPORT void
_clip_destroy_locals(ClipMachine * ClipMachineMemory)
{
   if (ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame)
   {
      if (ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame->refcount_of_ClipVarFrame == 0)
      {
	 int i;

	 for (i = 0; i < ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame->size_of_ClipVarFrame; i++)
	    _clip_destroy(ClipMachineMemory, ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame->ClipVar_vars_of_ClipVarFrame + i);
      }
   }
}
