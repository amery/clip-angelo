int
clip_INIT_RDD(ClipMachine * ClipMachineMemory)
{
   strncpy(ClipMachineMemory->def_data_driver, "DBF", 3);
   strncpy(ClipMachineMemory->def_idx_driver, "NTX", 3);
   strncpy(ClipMachineMemory->def_memo_driver, "DBT", 3);
   strncpy(ClipMachineMemory->def_db_driver, "DBFNTX", 6);
   return 0;
}
