/* LOAD(pobj-name [, array-for-names]) -> Logical */
int
clip_LOAD(ClipMachine * ClipMachineMemory)
{
   char *name = _clip_parc(ClipMachineMemory, 1);

   ClipVar *ap = _clip_par(ClipMachineMemory, 2);

   int r;

   Coll names;

   if (!name)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "no name given");
      return 1;
   }
   init_Coll(&names, free, 0);

   r = _clip_load(ClipMachineMemory, name, &names, 0);
   _clip_retl(ClipMachineMemory, !r);

   if (!r && ap && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      int i;

		for (i = 0; i < names.count_of_Coll; i++)
      {
	 ClipVar v;

	 char *s = (char *) names.items_of_Coll[i];

	 _clip_var_str(s, strlen(s), &v);
	 _clip_aadd(ClipMachineMemory, ap, &v);
	 _clip_destroy(ClipMachineMemory, &v);
      }
   }

   destroy_Coll(&names);

   return r;
}
