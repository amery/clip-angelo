int
clip_RDDFIELDTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFIELDTYPE";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int fno = _clip_parni(ClipMachineMemory, 2) - 1;

   char type[2] = " ";

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (rd)
   {
      if (fno >= 0 && fno < rd->nfields)
	 type[0] = rd->fields[fno].type;
   }

   _clip_retc(ClipMachineMemory, type);
   return 0;
 err:
   return er;
}
