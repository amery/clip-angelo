int
clip_ORDBAGNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDBAGNAME";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   int ord, er;

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   ord = get_orderno(wa, order, NULL);
   if (ord == -1)
   {
      if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType || order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar == 0)
	 ord = wa->rd->curord;
   }
   if (ord == -1)
      return 0;

   _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->index->name);
   return 0;
 err:
   return er;
}
