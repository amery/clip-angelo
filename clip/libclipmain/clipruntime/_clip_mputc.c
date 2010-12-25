CLIP_DLLEXPORT int
_clip_mputc(ClipMachine * ClipMachineMemory, ClipVar * ap, long no, char *str, int len)
{
   ClipVar v;

   int r;

   memset(&v, 0, sizeof(v));
   _clip_var_str(str, len, &v);
   r = _clip_madd(ClipMachineMemory, ap, no, &v);
   _clip_destroy(ClipMachineMemory, &v);

   return r;
}
