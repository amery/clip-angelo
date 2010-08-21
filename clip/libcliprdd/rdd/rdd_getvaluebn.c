int
rdd_getvaluebn(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *fname, const char *__PROC__)
{
   int fno = _rdd_fieldno(rd, _clip_casehashword(fname, strlen(fname)));

   if (fno < 0)
      return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   return rdd_getvalue(ClipMachineMemory, rd, fno, __PROC__);
}
