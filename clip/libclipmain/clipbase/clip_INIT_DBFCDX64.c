int
clip_INIT_DBFCDX64(ClipMachine * ClipMachineMemory)
{
   register_dbdriver(ClipMachineMemory, "DBFCDX64", "DBF64", "CDX", "FPT");
   return 0;
}
