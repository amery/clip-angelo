int
rdd_setvaluebn(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *fname, ClipVar * vp, const char *__PROC__)
{
   int fno = _rdd_fieldno(rd, _clip_casehashword(fname, strlen(fname)));

   if (fno < 0)
      return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   return rdd_setvalue(ClipMachineMemory, rd, fno, vp, __PROC__);
}
