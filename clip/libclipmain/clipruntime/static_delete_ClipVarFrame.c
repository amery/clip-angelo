static void
delete_ClipVarFrame(ClipMachine * ClipMachineMemory, ClipVarFrame * localvars)
{
   if (localvars && localvars->refcount_of_ClipVarFrame > 0 && ((--(localvars->refcount_of_ClipVarFrame)) < 1))
   {
      int i;

      for (i = 0; i < localvars->size_of_ClipVarFrame; i++)
	 _clip_destroy(ClipMachineMemory, localvars->ClipVar_vars_of_ClipVarFrame + i);
      free(localvars);
   }
}
