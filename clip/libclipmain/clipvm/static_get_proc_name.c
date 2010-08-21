static char *
get_proc_name(ClipCodeVar * dest)
{
   int nprivates, nlocals, nreflocals, numlocals;

   char *procname, *func;

   if (dest->ClipType_type_of_ClipCodeVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType)
      return "";

   func = dest->union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar->function_of_ClipBlock;
   nlocals = GETSHORT(F_OFFS(func, 3, 0, 1));
   nreflocals = GETSHORT(F_OFFS(func, 3, 1, 1));
   numlocals = nlocals + nreflocals;
   nprivates = GETSHORT(F_OFFS(func, 3, 3, 1));
   procname = F_OFFS(func, 3 + nprivates + numlocals, 5 + numlocals, 1 + 1);

   return procname;
}
