int
clip_HS_CREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_CREATE";

   const char *fname = _clip_parc(ClipMachineMemory, 1);

   int pagesize = _clip_parni(ClipMachineMemory, 2) * 1024;

   int lcase = _clip_parl(ClipMachineMemory, 4);

   int filtset = _clip_parni(ClipMachineMemory, 5);

   HIPER *hs;

   int er;

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(5, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      pagesize = getpagesize();
   if (_clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
      lcase = 0;
   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      filtset = 1;

   if ((er = hs_create(ClipMachineMemory, fname, pagesize, lcase, filtset, &hs, __PROC__)))
      goto err;
   hs->map = calloc(1, 4);
   _clip_retni(ClipMachineMemory, _clip_store_c_item(ClipMachineMemory, (void *) hs, _C_ITEM_TYPE_HIPER, destroy_hiper));
   return 0;
 err:
   return er;
}
