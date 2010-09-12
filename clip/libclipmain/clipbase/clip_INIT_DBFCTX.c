int
clip_INIT_DBFCTX(ClipMachine * ClipMachineMemory)
{
   register_dbdriver(ClipMachineMemory, "DBFCTX", "DBF", "CTX", "DBT");
   return 0;
}
