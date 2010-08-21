CLIP_DLLEXPORT int
_clip_type(ClipVar * vp)
{
   int r = 0;

   if (vp)
      r = _clip_vptr(vp)->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
   return r;
}
