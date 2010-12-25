CLIP_DLLEXPORT void
_clip_trap_str(ClipMachine * ClipMachineMemory, const char *filename, int line, const char *str)
{
   ClipVar *vp;

   vp = _clip_new_error(ClipMachineMemory, _clip_gettext(str), 2, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   _clip_trap_var(ClipMachineMemory, filename, line, vp);
}
