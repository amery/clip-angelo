static int
calc_loopcount(ClipMachine * ClipMachineMemory, ClipVar * vp, ClipVarFrame * vfp, int deep)
{
   int r = 0, i;

   vp = _clip_vptr(vp);

   if (deep > MAX_LOOP_DEEP)
      return 0;
   deep++;

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CCODE_type_of_ClipVarType:
   case PCODE_type_of_ClipVarType:
      if (vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar == vfp)
	 r = 1;
      break;
   case MAP_type_of_ClipVarType:
      for (i = 0; i < vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar; i++)
      {
	 ClipVar *vpp = &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl);

	 r += calc_loopcount(ClipMachineMemory, vpp, vfp, deep);
      }
      break;
   case ARRAY_type_of_ClipVarType:
      for (i = 0; i < vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
      {
	 ClipVar *vpp = vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	 r += calc_loopcount(ClipMachineMemory, vpp, vfp, deep);
      }
      break;
   default:
      break;
   }
   return r;
}
