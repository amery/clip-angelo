int
clip_RDDFIELDSIZE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDSIZE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int fno = _clip_parni(ClipMachineMemory, 2) - 1;

   int len = 0, er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (rd)
   {
      if (fno >= 0 && fno < rd->nfields)
	 len = rd->fields[fno].len;
   }
   _clip_retni(ClipMachineMemory, len);
   return 0;
 err:
   return er;
}
