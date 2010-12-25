int
clip_FIELDDECI(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDDECI";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int fno = _clip_parni(ClipMachineMemory, 1) - 1;

   int dec = 0, er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (wa)
   {
      if (fno >= 0 && fno < wa->rd->nfields)
	 dec = wa->rd->fields[fno].dec;
   }
   _clip_retni(ClipMachineMemory, dec);
   return 0;
 err:
   return er;
}
