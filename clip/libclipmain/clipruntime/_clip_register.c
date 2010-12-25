CLIP_DLLEXPORT int
_clip_register(ClipMachine * ClipMachineMemory, ClipFunction func, const char *name)
{
   return _clip_register_hash(ClipMachineMemory, func, _clip_hashstr(name));
}
