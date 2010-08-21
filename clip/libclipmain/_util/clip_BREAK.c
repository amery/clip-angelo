int
clip_BREAK(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_spar(ClipMachineMemory, 1);

   ClipVar *np = NEW(ClipVar);

   if (!vp)
   {
      _clip_trap_str(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, "BREAK function requare argument");
      return EG_ARG;
   }

   _clip_clone(ClipMachineMemory, np, vp);
   _clip_trap_var(ClipMachineMemory, ClipMachineMemory->fp->filename_of_ClipFrame, ClipMachineMemory->fp->line_of_ClipFrame, np);

   return -1;
}
