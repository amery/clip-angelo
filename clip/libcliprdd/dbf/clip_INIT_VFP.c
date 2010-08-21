int
clip_INIT_VFP(ClipMachine * ClipMachineMemory)
{
   rdd_registerdatadriver(ClipMachineMemory, vfp_vtbl());
   return 0;
}
