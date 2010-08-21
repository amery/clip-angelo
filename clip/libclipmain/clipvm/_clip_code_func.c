CLIP_DLLEXPORT int
_clip_code_func(ClipMachine * ClipMachineMemory, struct ClipBlock *cp, int argc, int rest, ClipVarFrame * uplocals)
{
   return do_code(ClipMachineMemory, cp, argc, 0, rest, uplocals);
}
