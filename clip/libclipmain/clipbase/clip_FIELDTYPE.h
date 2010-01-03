int
clip_FIELDTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDTYPE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       fno = _clip_parni(ClipMachineMemory, 1) - 1;

   char      type[2] = " ";

   int       er;


   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (wa)
    {
       if (fno >= 0 && fno < wa->rd->nfields)
	  type[0] = wa->rd->fields[fno].type;
    }

   _clip_retc(ClipMachineMemory, type);
   return 0;
 err:
   return er;
}
