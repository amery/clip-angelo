static int
cdx_ii_addkey(ClipMachine * ClipMachineMemory, RDD_ORDER * ro, const char *cid, ClipVar * key, const char *__PROC__)
{
   unsigned int id = *(unsigned int *) cid;

   int er;

   if ((er = __cdx_addkey(ClipMachineMemory, -1, ro, key, ro->index->loc, id, __PROC__)))
      return er;
   memcpy(ro->iikey, ro->key, ro->bufsize);
   if ((ro->type == 'C' || (ro->type == 'X' && ((char *) ro->iikey)[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_read(ro->index->loc, (unsigned char *) ro->iikey, ro->bufsize);
   memcpy(ro->id, &id, sizeof(unsigned int));
   return 0;
}
