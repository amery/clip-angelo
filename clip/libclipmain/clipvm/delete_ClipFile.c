void
delete_ClipFile(ClipMachine * ClipMachineMemory, ClipFile * fp)
{
   if (destroy_ClipFile(ClipMachineMemory, fp))
      free(fp);
}
