CLIP_DLLEXPORT int
_clip_func(ClipMachine * ClipMachineMemory, ClipFunction f, int argc, int rest, ClipVarFrame * uplocals)
{
   int r;

   ClipFrame *inMacro = ClipMachineMemory->inMacro;

   ClipMachineMemory->inMacro = 0;
   r = do_call(ClipMachineMemory, f, argc, 0, rest, uplocals);
   ClipMachineMemory->inMacro = inMacro;
   return r;
}
