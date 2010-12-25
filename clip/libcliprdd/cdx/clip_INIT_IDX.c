int
clip_INIT_IDX(ClipMachine * ClipMachineMemory)
{
   rdd_registerindexdriver(ClipMachineMemory, idx_vtbl());
   return 0;
}
