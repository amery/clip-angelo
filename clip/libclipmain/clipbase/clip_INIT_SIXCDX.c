int
clip_INIT_SIXCDX(ClipMachine * ClipMachineMemory)
{
   register_dbdriver(ClipMachineMemory, "SIXCDX", "FOX", "CDX", "FPT");
   return 0;
}
