static DepNode *
new_DepNode(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   DepNode *np;

   ClipVar *ap;

   np = NEW(DepNode);
   np->vp = vp;
   np->setno = -1;

   ap = _clip_vptr(vp);
   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
   {
      np->hash = _clip_hash(ClipMachineMemory, ap);
      return np;
   }

   if (ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar > 0)
      np->hash = _clip_hash(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 0);
   else
      np->hash = (long) ap;

   if (ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar > 1)
   {
      ClipVar *dp = _clip_vptr(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 1);

      if (dp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
      {
	 int i;

	 np->ndeps = dp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	 np->deps = (long *) calloc(np->ndeps, sizeof(long));

	 for (i = 0; i < np->ndeps; i++)
	    np->deps[i] = _clip_hash(ClipMachineMemory, dp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);
      }
   }

   return np;
}
