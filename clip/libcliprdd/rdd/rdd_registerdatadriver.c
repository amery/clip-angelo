void
rdd_registerdatadriver(ClipMachine * ClipMachineMemory, RDD_DATA_VTBL * vtbl)
{
   if (!*ClipMachineMemory->ndata_drivers)
      *ClipMachineMemory->data_drivers = (RDD_DATA_VTBL **) malloc(sizeof(RDD_DATA_VTBL *));
   else
      *ClipMachineMemory->data_drivers = (RDD_DATA_VTBL **) realloc(*ClipMachineMemory->data_drivers, sizeof(RDD_DATA_VTBL *) * (*ClipMachineMemory->ndata_drivers + 1));
   (*ClipMachineMemory->ndata_drivers)++;
   (*ClipMachineMemory->data_drivers)[*ClipMachineMemory->ndata_drivers - 1] = vtbl;
}
