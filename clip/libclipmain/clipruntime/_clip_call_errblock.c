CLIP_DLLEXPORT int
_clip_call_errblock(ClipMachine * ClipMachineMemory, int ret)
{
   if (!ret)
      return 0;

   if (!ClipMachineMemory->trapVar)
      _clip_generr(ClipMachineMemory, ret);

   if (ret < 0)
      return ret;

   if (!_clip_errorblock(ClipMachineMemory, ClipMachineMemory->trapVar, ret))
      ret = -ret;
   return ret;
}
