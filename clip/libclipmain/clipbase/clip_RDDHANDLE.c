int
clip_RDDHANDLE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDHANDLE";

   int area = _clip_parni(ClipMachineMemory, 1);

   DBWorkArea *wa = NULL;

   int er;

   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, -1);
   if (_clip_parinfo(ClipMachineMemory, 1) != UNDEF_type_of_ClipVarType)
      wa = get_area(ClipMachineMemory, area, 0, 0);
   if (!wa)
      wa = cur_area(ClipMachineMemory);
   if (wa)
      _clip_retni(ClipMachineMemory, wa->rd_item);
   return 0;
 err:
   return er;
}
