int
clip_FIELDSIZE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDSIZE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       fno = _clip_parni(ClipMachineMemory, 1) - 1;

   int       len = 0, er;


   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (wa)
    {
       if (fno >= 0 && fno < wa->rd->nfields)
	  len = wa->rd->fields[fno].len;
    }
   _clip_retni(ClipMachineMemory, len);
   return 0;
 err:
   return er;
}
