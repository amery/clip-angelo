static int
dbf_setstruct(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   return rdd_write(ClipMachineMemory, &rd->file, 28, 1, "\001", __PROC__);
}
