int
clip_ADEL(ClipMachine * ClipMachineMemory)
{
   int r;

   long n;

   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *vp = _clip_par(ClipMachineMemory, 2);

   if (!ap || !vp)
      return EG_ARG;

   n = _clip_hash(ClipMachineMemory, vp);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      long vect = n - 1;

      r = _clip_adel(ClipMachineMemory, ap, 1, &vect);
      if (r)
	 return r;
   }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
   {
      r = _clip_mdel(ClipMachineMemory, ap, n);
      if (r)
	 return r;
   }
   else
      return EG_ARG;

   _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ARGPTR(ClipMachineMemory, 1));

   return 0;
}
