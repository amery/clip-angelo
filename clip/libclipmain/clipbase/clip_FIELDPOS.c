int
clip_FIELDPOS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDPOS";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   const char *name = _clip_parc(ClipMachineMemory, 1);

   int er;

   _clip_retni(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, _rdd_fieldno(wa->rd, _clip_casehashword(name, strlen(name))) + 1);
   return 0;
 err:
   return er;
}
