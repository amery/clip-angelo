CLIP_DLLEXPORT void
_clip_generr(ClipMachine * ClipMachineMemory, int code)
{
   if (ClipMachineMemory->trapVar)
      return;

   if (code < 0)
      code = -code;

   ClipMachineMemory->trapVar = _clip_new_error(ClipMachineMemory, _clip_errname(code), 2, code, 0, 0, 0, 0, 0, 0, 0, 0);
}
