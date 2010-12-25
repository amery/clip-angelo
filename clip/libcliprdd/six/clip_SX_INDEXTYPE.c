int
clip_SX_INDEXTYPE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_INDEXTYPE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *index = _clip_par(ClipMachineMemory, 1);

   int ord, er;

   ClipVar order;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   memset(&order, 0, sizeof(ClipVar));
   order.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   order.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 1;
   if (!index)
      ord = -1;
   else
      ord = get_orderno(wa, &order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;
   _clip_retni(ClipMachineMemory, wa->rd->orders[ord]->vtbl->ismulti ? 3 : 2);
   return 0;
 err:
   return er;
}
