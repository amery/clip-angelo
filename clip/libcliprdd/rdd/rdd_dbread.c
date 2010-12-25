int
rdd_dbread(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * ap, const char *__PROC__)
{
   ClipVar vv;

   int i, er;

   CLEAR_CLIPVAR(&vv);

   _clip_map(ClipMachineMemory, ap);
   for (i = 0; i < rd->nfields; i++)
   {
      if ((er = rdd_takevalue(ClipMachineMemory, rd, i, &vv, __PROC__)))
	 return er;
      _clip_madd(ClipMachineMemory, ap, _clip_casehashword(rd->fields[i].name, strlen(rd->fields[i].name)), &vv);
      _clip_destroy(ClipMachineMemory, &vv);
   }
   return 0;
}
