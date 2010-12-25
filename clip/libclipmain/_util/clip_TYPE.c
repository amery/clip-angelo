int
clip_TYPE(ClipMachine * ClipMachineMemory)
{
   int len;

   char *str = _clip_parcl(ClipMachineMemory, 1, &len);

   char *rc = "UI";

   ClipVar var;

   int r;

   if (!str)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "TYPE");
   }
   else
   {
      char *s, *e;

      for (s = str, e = str + len; s < e; ++s)
	 if (*s == '(' || *s == ')')
	 {
	    _clip_retc(ClipMachineMemory, rc);
	    return 0;
	 }

      var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
      var.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      ClipMachineMemory->noerrblock++;
      r = _clip_eval_macro(ClipMachineMemory, str, len, &var);
      ClipMachineMemory->noerrblock--;
      if (r < 0)
	 rc = "UE";
      else if (r > 0)
	 rc = "U";
      else
	 rc = _clip_type_name(&var);
      _clip_destroy(ClipMachineMemory, &var);
   }

   _clip_retc(ClipMachineMemory, rc);
   return 0;
}
