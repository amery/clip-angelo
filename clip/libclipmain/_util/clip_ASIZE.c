int
clip_ASIZE(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   int nl = _clip_parni(ClipMachineMemory, 2);

   int r;

   ClipVar *rp;

   if (!ap)
      return 0;

   if (nl < 0)
      nl = 0;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      long vect = nl;

      r = _clip_asize(ClipMachineMemory, ap, 1, &vect);
      if (r)
	 return r;
   }

   rp = RETPTR(ClipMachineMemory);
   _clip_clone(ClipMachineMemory, rp, ARGPTR(ClipMachineMemory, 1));

   return 0;
}
