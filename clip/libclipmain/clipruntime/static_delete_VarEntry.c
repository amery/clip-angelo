static void
delete_VarEntry(ClipMachine * ClipMachineMemory, VarEntry * vp)
{
   if (!vp)
      return;
   if (vp->VarEntry_next_of_VarEntry)
      delete_VarEntry(ClipMachineMemory, vp->VarEntry_next_of_VarEntry);
   _clip_destroy(ClipMachineMemory, &vp->ClipVar_var_of_VarEntry);
   free(vp);
}
