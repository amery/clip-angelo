int
clip_ORDISUNIQUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDISUNIQUE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   int ord, er;

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   _clip_retl(ClipMachineMemory, wa->rd->orders[ord]->unique);
   return 0;
 err:
   return er;
}
