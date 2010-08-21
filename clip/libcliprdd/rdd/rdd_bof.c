int
rdd_bof(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *bof, const char *__PROC__)
{
   *bof = rd->bof | rd->v_bof;
   return 0;
}
