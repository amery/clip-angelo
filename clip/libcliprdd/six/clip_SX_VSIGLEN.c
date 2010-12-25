int
clip_SX_VSIGLEN(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_VSIGLEN";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int t = _clip_parinfo(ClipMachineMemory, 1);

   int fno = -1, er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);

   if (!wa)
      return 0;

   if (t == CHARACTER_type_of_ClipVarType)
   {
      char *fname = _clip_parc(ClipMachineMemory, 1);

      if (fname)
	 fno = _rdd_fieldno(wa->rd, _clip_casehashword(fname, strlen(fname)));
   }
   else
   {
      fno = _clip_parni(ClipMachineMemory, 1) - 1;
   }

   if (fno < 0 || fno >= wa->rd->nfields)
      return 0;

   if (wa->rd->fields[fno].type == 'V' && wa->rd->fields[fno].len != 3 && wa->rd->fields[fno].len != 4)
   {
      _clip_retni(ClipMachineMemory, max(wa->rd->fields[fno].len - 6, 0));
   }
   else
   {
      _clip_retni(ClipMachineMemory, wa->rd->fields[fno].len);
   }
   return 0;
 err:
   return er;
}
