CLIP_DLLEXPORT int
_clip_parn(ClipMachine * ClipMachineMemory)
{
   int no;

   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   no = _clip_long(sp);

   _clip_destroy(ClipMachineMemory, sp);

   if (no < 1 || no > ClipMachineMemory->argc)
      return 0;

   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame--;
   _clip_push(ClipMachineMemory, ARGPTR(ClipMachineMemory, no));
   return 0;
}
