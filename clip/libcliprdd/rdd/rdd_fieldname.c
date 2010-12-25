int
rdd_fieldname(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int fno, const char *__PROC__)
{
   if (fno < 0 || fno >= rd->nfields)
      _clip_retc(ClipMachineMemory, "");

   _clip_retc(ClipMachineMemory, rd->fields[fno].name);
   return 0;
}
