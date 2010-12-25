CLIP_DLLEXPORT void
_clip_trap_var(ClipMachine * ClipMachineMemory, const char *filename, int line, ClipVar * vp)
{
   _clip_delete(ClipMachineMemory, ClipMachineMemory->trapVar);
   _clear_trap(ClipMachineMemory);
   if (vp)
   {
      /*vp = _clip_vptr(vp); */
      ClipMachineMemory->trapVar = vp;
   }
   else
   {
      ClipMachineMemory->trapVar = NEW(ClipVar);
   }
   _clear_trap(ClipMachineMemory);
   _clip_trap(ClipMachineMemory, filename, line);
}
