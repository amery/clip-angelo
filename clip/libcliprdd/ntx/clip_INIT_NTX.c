int
clip_INIT_NTX(ClipMachine * ClipMachineMemory)
{
   rdd_registerindexdriver(ClipMachineMemory, ntx_vtbl());
   return 0;
}
