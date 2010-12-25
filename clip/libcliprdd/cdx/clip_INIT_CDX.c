int
clip_INIT_CDX(ClipMachine * ClipMachineMemory)
{
   rdd_registerindexdriver(ClipMachineMemory, cdx_vtbl());
   return 0;
}
