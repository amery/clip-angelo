int
clip___DBSETFOUND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__DBSETFOUND";

   int f = _clip_parl(ClipMachineMemory, 1);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int er;

   CHECKWA(wa);
   CHECKOPT1(1, LOGICAL_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, wa->found ? 1 : 0);
   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      wa->found = f;
   return 0;
 err:
   return er;
}
