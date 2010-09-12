int
clip_INDEXKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "INDEXKEY";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int ord = _clip_parni(ClipMachineMemory, 1);

   int er;

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (ord == 0)
      ord = wa->rd->curord + 1;

   if (ord < 1 || ord > wa->rd->ords_opened)
      return 0;

   _clip_retc(ClipMachineMemory, wa->rd->orders[ord - 1]->expr);
   return 0;
 err:
   return er;
}
