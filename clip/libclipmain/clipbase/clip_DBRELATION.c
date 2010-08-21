int
clip_DBRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRELATION";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int rel = _clip_parni(ClipMachineMemory, 1);

   int er;

   _clip_retc(ClipMachineMemory, "");
   if (!wa)
      return 0;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (!(rel < 1 || rel > wa->rd->rels_opened))
      _clip_retc(ClipMachineMemory, wa->rd->relations[rel - 1]->expr);
   return 0;
 err:
   return er;
}
