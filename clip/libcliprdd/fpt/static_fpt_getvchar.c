static int
fpt_getvchar(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, int len, unsigned int id, char *buf, const char *__PROC__)
{
   return rdd_read(ClipMachineMemory, &rm->file, id * rm->blocksize, len, buf, __PROC__);
}
