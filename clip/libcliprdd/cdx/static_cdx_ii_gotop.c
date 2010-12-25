static int
cdx_ii_gotop(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *__PROC__)
{
   int out, er;

   ro->bof = ro->eof = 0;
   if ((er = _cdx_first(ClipMachineMemory, ro, &out, __PROC__)))
      return er;
   if (out)
   {
      ro->bof = ro->eof = 1;
      return 0;
   }
   if ((er = _cdx_recno_keyval(ClipMachineMemory, ro, ro->level, (unsigned int *) ro->id, ro->iikey, __PROC__)))
      return er;
   if ((ro->type == 'C' || (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
   return 0;
}
