static int
descr_str(ClipMachine * ClipMachineMemory, ClipVar * vp, char **str)
{
   int l;

   if (!vp)
      return 0;
   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      return _clip_strFromVar(ClipMachineMemory, vp, str, &l);
      break;
   case MAP_type_of_ClipVarType:
      {
	 int ind, l;

	 if (!_clip_mind(ClipMachineMemory, vp, HASH_description, &ind))
	 {
	    ClipVar *ep, *ap;

	    ap = _clip_vptr(vp);

	    ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
	    return _clip_strFromVar(ClipMachineMemory, ep, str, &l);
	 }
      }
      break;
   default:
      break;
   }
   return 0;

}
