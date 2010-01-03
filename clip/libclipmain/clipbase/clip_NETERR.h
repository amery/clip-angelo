int
clip_NETERR(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "NETERR";

   int      *err, r;

   int       neterr = _clip_parl(ClipMachineMemory, 1);

   int       er;


   CHECKOPT1(1, LOGICAL_type_of_ClipVarType);
   err = &ClipMachineMemory->neterr;
   r = *err;

   if (_clip_parinfo(ClipMachineMemory, 1) != UNDEF_type_of_ClipVarType)
      *err = neterr;

   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}
