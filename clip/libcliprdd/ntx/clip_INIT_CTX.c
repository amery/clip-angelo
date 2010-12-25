int
clip_INIT_CTX(ClipMachine * ClipMachineMemory)
{
   rdd_registerindexdriver(ClipMachineMemory, ctx_vtbl());
   return 0;
}
