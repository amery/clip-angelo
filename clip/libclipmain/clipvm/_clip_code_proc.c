CLIP_DLLEXPORT int
_clip_code_proc(struct ClipMachine *ClipMachineMemory, struct ClipBlock *cp, int argc, int rest, ClipVarFrame * uplocals)
{
   return do_code(ClipMachineMemory, cp, argc, 1, rest, uplocals);
}
