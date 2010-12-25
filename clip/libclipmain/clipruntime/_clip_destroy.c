CLIP_DLLEXPORT void
_clip_destroy(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   if (!vp)
      return;

   switch (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
   {
   case F_MPTR_ClipFlags:
   case F_MREF_ClipFlags:
      if ((--(vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType)) <= 0)
      {
	 _clip_destroy(ClipMachineMemory, vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
	 free(vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
      }
      CLEAR_CLIPVAR(vp);
      return;
   case F_MSTAT_ClipFlags:
      return;
   case F_NONE_ClipFlags:
      break;
   }

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
      break;
   case NUMERIC_type_of_ClipVarType:
      if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
      {
	 rational_destroy(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      }
      break;
   case ARRAY_type_of_ClipVarType:
      {
	 int i;

	 for (i = vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar - 1; i >= 0; --i)
	    _clip_destroy(ClipMachineMemory, vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);
	 free(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar);
      }
      break;
   case MAP_type_of_ClipVarType:
      {
	 int i;

#if 1
	 int ind;

	 /* try run `destroy` member */
	 if (search_map(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, HASH_destroy, &ind))
	 {
	    ClipVar *ep;

	    int type;

	    ep = &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
	    type = _clip_type(ep);

	    if (type == PCODE_type_of_ClipVarType || type == CCODE_type_of_ClipVarType)
	    {
	       ClipVar vpp;

	       memset(&vpp, 0, sizeof(vpp));
	       vpp.ClipType_t_of_ClipVar = vp->ClipType_t_of_ClipVar;
	       vpp.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	       vpp.ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
	       vp->ClipType_t_of_ClipVar.count_of_ClipType++;
	       _clip_eval(ClipMachineMemory, _clip_vptr(ep), 1, &vpp, 0);
	    }
	 }
#endif

	 for (i = vp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar - 1; i >= 0; --i)
	    _clip_destroy(ClipMachineMemory, &(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl));
	 free(vp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar);
      }
      break;
   case CCODE_type_of_ClipVarType:

      delete_ClipVarFrame(ClipMachineMemory, vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);
      break;

   case PCODE_type_of_ClipVarType:

      delete_ClipVarFrame(ClipMachineMemory, vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar);
      destroy_Block(ClipMachineMemory, vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar);
      free(vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar);

      break;
   default:
      break;
   }
   CLEAR_CLIPVAR(vp);
}
