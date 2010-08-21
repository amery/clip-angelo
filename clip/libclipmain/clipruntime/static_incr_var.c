static int
incr_var(ClipMachine * ClipMachineMemory, ClipVar * vp, int isIncr)
{
   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case NUMERIC_type_of_ClipVarType:
      if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
      {
	 rational *tmp = rational_long_init((long) 1);

	 rational_adda(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar, tmp);
	 rational_destroy(tmp);
      }
      else
	 vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar += (isIncr ? 1 : -1);
      return 0;
   case DATE_type_of_ClipVarType:
      vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar += (isIncr ? 1 : -1);
      return 0;
   default:
      _clip_trap_str(ClipMachineMemory, __file__, __LINE__, "invalid type in INCR/DECR operation");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
}
