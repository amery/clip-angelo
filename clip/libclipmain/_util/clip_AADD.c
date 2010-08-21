int
clip_AADD(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *vp = _clip_spar(ClipMachineMemory, 2);

   long no;

   int c;

   ClipVar *vp3 = _clip_par(ClipMachineMemory, 3);

   if (!ap || !vp)
      return EG_ARG;

   if (vp3)
      no = _clip_hash(ClipMachineMemory, vp3);
   else
      no = 0;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      c = ++ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = (ClipVar *) realloc(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, sizeof(ClipVar) * c);
      memset(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, 0, sizeof(ClipVar));
      _clip_clone(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + c - 1, vp);
      _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), vp);
   }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
   {
      if (!no)
	 no = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
      _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), ARGPTR(ClipMachineMemory, 1));
      return _clip_madd(ClipMachineMemory, ap, no, vp);
   }
   else
      return EG_ARG;

   return 0;
}
