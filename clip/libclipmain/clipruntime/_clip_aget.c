CLIP_DLLEXPORT int
_clip_aget(ClipMachine * ClipMachineMemory, ClipVar * ap, ClipVar * resp, int ndim, long *dims)
{
   ClipVar *app;

   app = _clip_aref(ClipMachineMemory, ap, ndim, dims);
   if (app)
      return _clip_clone(ClipMachineMemory, resp, app);
   else
      return EG_NOVAR;
}
