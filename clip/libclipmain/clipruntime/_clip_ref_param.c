CLIP_DLLEXPORT ClipVar *
_clip_ref_param(ClipMachine * ClipMachineMemory, int no, ClipVar * params)
{
   if (no >= ClipMachineMemory->argc)
      return params + no;
   else
      return ClipMachineMemory->bp - (ClipMachineMemory->argc - no);
}
