int
clip_INIT_FPT(ClipMachine * ClipMachineMemory)
{
   rdd_registermemodriver(ClipMachineMemory, fpt_vtbl());
   return 0;
}
