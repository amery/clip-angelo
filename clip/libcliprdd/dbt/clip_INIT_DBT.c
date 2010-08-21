int
clip_INIT_DBT(ClipMachine * ClipMachineMemory)
{
   rdd_registermemodriver(ClipMachineMemory, dbt_vtbl());
   return 0;
}
