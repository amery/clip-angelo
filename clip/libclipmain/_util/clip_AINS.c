int
clip_AINS(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *vp1 = _clip_par(ClipMachineMemory, 2);

   ClipVar *vp = _clip_spar(ClipMachineMemory, 3);

   int r;

   long n;

   if (!ap || !vp1)
      return 0;

   n = _clip_hash(ClipMachineMemory, vp1);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      long vect = n - 1;

      r = _clip_ains(ClipMachineMemory, ap, 1, &vect);
      if (r)
	 return r;
      if (vp)
	 _clip_clone(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + n - 1, vp);
   }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
   {
      ClipVar zero;

      memset(&zero, 0, sizeof(zero));

      if (vp)
	 r = _clip_madd(ClipMachineMemory, ap, n, vp);
      else
	 r = _clip_madd(ClipMachineMemory, ap, n, &zero);
      if (r)
	 return r;
   }
   else
      return EG_ARG;

   _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ARGPTR(ClipMachineMemory, 1));
   return 0;
}
