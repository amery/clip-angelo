CLIP_DLLEXPORT int
_clip_trap_operation(ClipMachine * ClipMachineMemory, const char *filename, int line, const char *oper, ClipVar * lval)
{
   ClipVar *vp;

   int r;

   char buf[256];

   snprintf(buf, 256, _clip_gettext("invalid arguments for  '%s' operation"), oper);

   vp = _clip_new_error(ClipMachineMemory, buf, 2, 0, 0, 0, 0, 0, 0, 0, 0, oper);
   _clip_trap_var(ClipMachineMemory, filename, line, vp);

   if (!ClipMachineMemory->trapVar)
      _clip_generr(ClipMachineMemory, 1);

   _clip_destroy(ClipMachineMemory, lval);
   r = _clip_errorblock_res(ClipMachineMemory, ClipMachineMemory->trapVar, 1, lval);

   if (!r && _clip_type(lval) == UNDEF_type_of_ClipVarType)
      r = 1;

   return r;
}
