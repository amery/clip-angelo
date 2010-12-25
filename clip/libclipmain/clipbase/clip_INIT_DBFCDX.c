int
clip_INIT_DBFCDX(ClipMachine * ClipMachineMemory)
{
   register_dbdriver(ClipMachineMemory, "DBFCDX", "FOX", "CDX", "FPT");
   return 0;
}
