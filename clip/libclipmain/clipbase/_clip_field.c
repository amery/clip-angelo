CLIP_DLLEXPORT int
_clip_field(ClipMachine * ClipMachineMemory, long hash, long area)
{
   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;

   DBWorkArea *wa;

   const char nm[] = "_clip_field";

   char buf[11];

   int no;

   switch (area)
   {
   case HASH_m:
   case HASH_memv:
   case HASH_memva:
   case HASH_memvar:
      return _clip_ref(ClipMachineMemory, _clip_ref_memvar(ClipMachineMemory, hash), 0);
   default:
      break;
   }

   wa = get_area(ClipMachineMemory, area, 0, 0);
   if (!wa)
   {
      ClipVar *ap = _clip_ref_memvar_noadd(ClipMachineMemory, area);

      if (ap)
      {
	 ClipVar *vp = _clip_mget(ClipMachineMemory, ap, hash);

	 if (vp)
	 {
	    _clip_push(ClipMachineMemory, vp);
	    return 0;
	 }
      }

      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, buf, er_nofield);
   }
   if ((no = _rdd_fieldno(wa->rd, hash)) == -1)
   {
      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      return rdd_err(ClipMachineMemory, EG_NOVAR, 0, __FILE__, __LINE__, buf, er_nofield);
   }
   sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
   sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);

   return _rdd_takevalue(ClipMachineMemory, wa, no, hash, sp, nm);
}
