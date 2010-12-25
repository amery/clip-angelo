int
clip_INIT_FOX(ClipMachine * ClipMachineMemory)
{
   rdd_registerdatadriver(ClipMachineMemory, fox_vtbl());
   return 0;
}
