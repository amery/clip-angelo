static int
_rdd_takevalue(ClipMachine * ClipMachineMemory, DBWorkArea * wa, int fno, long fhash, ClipVar * vv, const char *nm)
{
   ClipVar *vp;

   int r, er;

   if (vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
   {
      if ((!wa->rd->valid || wa->rd->pending_child_parent) && !(wa->rd->file.rlocked || wa->rd->file.wlocked))
      {

	 if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, nm)))
	    return er;
	 r = rdd_takevalue(ClipMachineMemory, wa->rd, fno, vv->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar, nm);
	 if ((er = wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, nm)))
	    return er;
      }
      else
      {
	 r = rdd_takevalue(ClipMachineMemory, wa->rd, fno, vv->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar, nm);
      }

      vv->ClipType_t_of_ClipVar.field_of_ClipType = 1;
      vv->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = _clip_get_fielddef(ClipMachineMemory, wa->aliasHash, fhash);
      return r;
   }

   if ((!wa->rd->valid || wa->rd->pending_child_parent) && !(wa->rd->file.rlocked || wa->rd->file.wlocked))
   {

      if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, nm)))
	 return er;
      r = rdd_takevalue(ClipMachineMemory, wa->rd, fno, vv, nm);
      if ((er = wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, nm)))
	 return er;
   }
   else
   {
      r = rdd_takevalue(ClipMachineMemory, wa->rd, fno, vv, nm);
   }
   if (r)
      return r;

   vp = NEW(ClipVar);
   *vp = *vv;
   vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
   CLEAR_CLIPVAR(vv);
   vv->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
   vv->ClipType_t_of_ClipVar.memo_of_ClipType = vp->ClipType_t_of_ClipVar.memo_of_ClipType;
   vv->ClipType_t_of_ClipVar.field_of_ClipType = 1;
   vv->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
   vv->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
   vv->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = _clip_get_fielddef(ClipMachineMemory, wa->aliasHash, fhash);

   return 0;
}
