int
clip_INIT_VFPCDX(ClipMachine * ClipMachineMemory)
{
   register_dbdriver(ClipMachineMemory, "VFPCDX", "VFP", "CDX", "FPT");
   return 0;
}
