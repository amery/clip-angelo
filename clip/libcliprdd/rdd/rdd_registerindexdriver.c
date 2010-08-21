void
rdd_registerindexdriver(ClipMachine * ClipMachineMemory, RDD_INDEX_VTBL * vtbl)
{
   if (!*ClipMachineMemory->nidx_drivers)
      *ClipMachineMemory->idx_drivers = (RDD_INDEX_VTBL **) malloc(sizeof(RDD_INDEX_VTBL *));
   else
      *ClipMachineMemory->idx_drivers = (RDD_INDEX_VTBL **) realloc(*ClipMachineMemory->idx_drivers, sizeof(RDD_INDEX_VTBL *) * (*ClipMachineMemory->nidx_drivers + 1));
   (*ClipMachineMemory->nidx_drivers)++;
   (*ClipMachineMemory->idx_drivers)[*ClipMachineMemory->nidx_drivers - 1] = vtbl;
}
