int
clip_ORDNUMBER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDNUMBER";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar  *order = _clip_par(ClipMachineMemory, 1);

   ClipVar  *index = _clip_par(ClipMachineMemory, 2);

   int       ord, er;


   if (!_clip_parinfo(ClipMachineMemory, 0))
      _clip_retni(ClipMachineMemory, wa->rd->curord + 1);
   else
      _clip_retni(ClipMachineMemory, 0);

   CHECKWA(wa);
   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return 0;

   _clip_retni(ClipMachineMemory, ord + 1);
   return 0;
 err:
   return er;
}
