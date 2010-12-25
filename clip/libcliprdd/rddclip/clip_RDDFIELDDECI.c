int
clip_RDDFIELDDECI(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDDECI";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int fno = _clip_parni(ClipMachineMemory, 2) - 1;

   int dec = 0, er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (rd)
   {
      if (fno >= 0 && fno < rd->nfields)
	 dec = rd->fields[fno].dec;
   }
   _clip_retni(ClipMachineMemory, dec);
   return 0;
 err:
   return er;
}
