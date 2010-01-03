int
clip_GETALIAS(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "GETALIAS";

   char     *alias = _clip_parc(ClipMachineMemory, 1);

   char     *ch;

   DBWorkArea *wa;

   int       i, er = EG_UNSUPPORTED;


   CHECKARG1(1, CHARACTER_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   for (ch = alias; *ch; ch++)
      *ch = toupper(*ch);
   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
    {
       wa = ClipMachineMemory->areas->items_of_ClipVect[i];
       if (!wa || !wa->used)
	  continue;
       if (strcmp(wa->alias, alias) == 0)
	{
	   _clip_retni(ClipMachineMemory, i + 1);
	   break;
	}
    }
   return 0;
 err:
   return er;
}
