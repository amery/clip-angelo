void
rdd_registermemodriver(ClipMachine * ClipMachineMemory, RDD_MEMO_VTBL * vtbl)
{
   if (!*ClipMachineMemory->nmemo_drivers)
      *ClipMachineMemory->memo_drivers = (RDD_MEMO_VTBL **) malloc(sizeof(RDD_MEMO_VTBL *));
   else
      *ClipMachineMemory->memo_drivers = (RDD_MEMO_VTBL **) realloc(*ClipMachineMemory->memo_drivers, sizeof(RDD_MEMO_VTBL *) * (*ClipMachineMemory->nmemo_drivers + 1));
   (*ClipMachineMemory->nmemo_drivers)++;
   (*ClipMachineMemory->memo_drivers)[*ClipMachineMemory->nmemo_drivers - 1] = vtbl;
}
