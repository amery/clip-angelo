CLIP_DLLEXPORT int
_clip_mputn(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, double n)
{
   ClipVar v;

   int r;

   memset(&v, 0, sizeof(v));
   _clip_var_num(n, &v);
   r = _clip_madd(ClipMachineMemory, ap, no, &v);
   _clip_destroy(ClipMachineMemory, &v);

   return r;
}
