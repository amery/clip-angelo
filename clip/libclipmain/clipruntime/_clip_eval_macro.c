CLIP_DLLEXPORT int
_clip_eval_macro(ClipMachine * ClipMachineMemory, char *str, int len, ClipVar * dest)
{
   char *s = str, *e;

   int simple = 1;

   if (len == 0)
   {
      dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
      dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      return 0;
   }

   if (!(isalpha(*s) || *s == '_'))
      simple = 0;
   else
      for (s = str + 1, e = str + len; s < e; ++s)
	 if (!(isalnum(*s) || *s == '_'))
	 {
	    simple = 0;
	    break;
	 }

   if (simple)
   {
      VarEntry *vp;

      long hash = _clip_casehashbytes(0, str, len);

      if (!_clip_take_field(ClipMachineMemory, hash, -1, dest))
	 return 0;

      vp = fetch_var(ClipMachineMemory, hash);

      if (!vp)
      {
	 if (ClipMachineMemory->obj)
	 {
	    ClipVar *vpp;

	    vpp = fetch_obj(ClipMachineMemory, _clip_vptr(ClipMachineMemory->obj), hash);
	    if (vpp)
	       return _clip_clone(ClipMachineMemory, dest, vpp);
	 }
      }

      if (!vp)
      {
	 if (ClipMachineMemory->noerrblock)
	 {
	    ClipVar var;

	    memset(&var, 0, sizeof(var));
	    return _clip_clone(ClipMachineMemory, dest, &var);
	 }
	 else
	 {
	    _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "no variable name: '%.*s'", len, str);
	    return _clip_call_errblock(ClipMachineMemory, 1);
	    /*vp = add_private(ClipMachineMemory, hash); */
	 }
      }
      return _clip_clone(ClipMachineMemory, dest, &vp->ClipVar_var_of_VarEntry);
   }
   else
   {
      ClipBlock block;

      int ret = _clip_compile_Block(ClipMachineMemory, str, len, &block);

      ClipVar stack[1];

      ClipFrame frame = { stack, stack + 1, __file__, __LINE__, 0, 0, 0, 0, 0, 0,
	 "eval_macro", 1, 0
      };
      ClipFrame *inMacro = ClipMachineMemory->inMacro;

      if (ret)
	 return -1;
      memset(stack, 0, sizeof(stack));

      ClipMachineMemory->inMacro = ClipMachineMemory->fp;
      frame.ClipFrame_up_of_ClipFrame = ClipMachineMemory->fp;
      ClipMachineMemory->fp = &frame;

      ret = _clip_code_func(ClipMachineMemory, &block, 0, 0, 0);

      ClipMachineMemory->inMacro = inMacro;
      ClipMachineMemory->fp = frame.ClipFrame_up_of_ClipFrame;
      *dest = stack[0];
      destroy_Block(ClipMachineMemory, &block);
      if (ret)
	 return 1;
      else
	 return 0;
   }
}
