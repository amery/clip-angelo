CLIP_DLLEXPORT int
_clip_iassign_field(ClipMachine * ClipMachineMemory, long hash, long area)
{
   ClipVar *sp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);

   DBWorkArea *wa;

   const char nm[] = "_clip_iassign_field";

   int fno, er;

   char buf[11];

   if (!(wa = get_area(ClipMachineMemory, area, 0, 0)))
   {
      if (area < 0)
      {
	 return rdd_err(ClipMachineMemory, EG_BADALIAS, 0, __FILE__, __LINE__, nm, er_badalias);
      }
      else
      {
#if 1
	 ClipVar *ap = _clip_ref_memvar_noadd(ClipMachineMemory, area);

	 if (ap)
	 {
	    ClipVar *vp = _clip_mget(ClipMachineMemory, ap, hash);

	    if (vp)
	    {
	       return _clip_iassign(ClipMachineMemory, vp);
	    }
	 }
	 return rdd_err(ClipMachineMemory, EG_BADALIAS, 0, __FILE__, __LINE__, nm, er_badalias);
#else
	 ClipVar *vp = space_var(ClipMachineMemory, __LINE__, area, hash);

	 if (!vp)
	    return 1;
	 else
	    return _clip_iassign(ClipMachineMemory, vp);
#endif
      }
   }

   if ((fno = _rdd_fieldno(wa->rd, hash)) == -1)
   {
      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, buf, er_nofield);
   }
   if ((!wa->rd->valid || wa->rd->pending_child_parent) && !(wa->rd->file.rlocked || wa->rd->file.wlocked))
   {

      if ((er = wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, nm)))
	 return er;
      if ((er = rdd_setvalue(ClipMachineMemory, wa->rd, fno, sp, nm)))
	 return er;
      if ((er = wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, nm)))
	 return er;
   }
   else
   {
      if ((er = rdd_setvalue(ClipMachineMemory, wa->rd, fno, sp, nm)))
	 return er;
   }
   return 0;
}
