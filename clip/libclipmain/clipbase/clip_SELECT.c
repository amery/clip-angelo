int
clip_SELECT(ClipMachine * ClipMachineMemory)
{
   const char *alias = _clip_parc(ClipMachineMemory, 1);

   DBWorkArea *wa;

   int i, len, ret = 0;

   char *al, *e;

   if (_clip_parinfo(ClipMachineMemory, 0) > 0 && _clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   if (!alias)
   {
      _clip_retni(ClipMachineMemory, ClipMachineMemory->curArea + 1);
      return 0;
   }

   al = strdup(alias);
   e = al + strlen(al);
   while (*(--e) == ' ');
   *(e + 1) = 0;
   e = al;
   while (isalpha(*e) || isdigit(*e) || *e == '_')
      e++;
   *e = 0;
   len = strlen(al);
   if (len > 10)
      len = 10;
   al[len] = 0;

   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
   {
      wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
      if (wa && wa->alias && !strcasecmp(al, wa->alias))
      {
	 ret = i + 1;
	 break;
      }
   }
   free(al);

   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
