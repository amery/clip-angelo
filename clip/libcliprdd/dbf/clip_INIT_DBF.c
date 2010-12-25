int
clip_INIT_DBF(ClipMachine * ClipMachineMemory)
{
   rdd_registerdatadriver(ClipMachineMemory, dbf_vtbl());
   return 0;
}
