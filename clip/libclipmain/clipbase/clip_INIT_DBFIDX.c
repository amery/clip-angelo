int
clip_INIT_DBFIDX(ClipMachine * ClipMachineMemory)
{
   register_dbdriver(ClipMachineMemory, "DBFIDX", "FOX", "IDX", "FPT");
   return 0;
}
