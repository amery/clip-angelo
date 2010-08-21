CLIP_DLLEXPORT void
_clip_trap_invargv(ClipMachine * ClipMachineMemory, const char *filename, int line)
{
   _clip_trap_str(ClipMachineMemory, filename, line, "invalid arguments");
}
