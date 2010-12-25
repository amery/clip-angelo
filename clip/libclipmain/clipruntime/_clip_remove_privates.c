CLIP_DLLEXPORT void
_clip_remove_privates(ClipMachine * ClipMachineMemory, int num, ... /*long hash */ )
{
   va_list ap;

   va_start(ap, num);
   remove_private_vect(ClipMachineMemory, num, ap);
   va_end(ap);
}
