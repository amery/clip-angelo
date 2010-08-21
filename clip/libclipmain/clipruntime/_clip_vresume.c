CLIP_DLLEXPORT void
_clip_vresume(ClipMachine * ClipMachineMemory, int num, ClipVar * vect)
{
   /*resume_mp(ClipMachineMemory); */
   for (; num; --num, ++vect)
      _clip_destroy(ClipMachineMemory, vect);
}
