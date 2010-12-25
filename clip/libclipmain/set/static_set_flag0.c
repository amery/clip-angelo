static void
set_flag0(ClipMachine * ClipMachineMemory, int flag, int beg)
{
   _clip_retl(ClipMachineMemory, ClipMachineMemory->flags & flag);
   if (ClipMachineMemory->argc > beg)
   {
      int lp = -1;

      ClipVar *vp;

      vp = _clip_par(ClipMachineMemory, beg + 1);
      switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case LOGICAL_type_of_ClipVarType:
	 lp = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	 break;
      case NUMERIC_type_of_ClipVarType:
	 lp = _clip_double(vp);
	 lp = (lp != 0);
	 break;
      case CHARACTER_type_of_ClipVarType:
	 {
	    char *s = _clip_parc(ClipMachineMemory, beg + 1);

	    if (!strcasecmp(s, "on"))
	       lp = 1;
	    else
	       lp = 0;
	 }
	 break;
      default:
	 break;
      }

      if (lp != -1)
      {
	 if (lp)
	    ClipMachineMemory->flags |= flag;
	 else
	    ClipMachineMemory->flags &= ~flag;
      }
   }
}
