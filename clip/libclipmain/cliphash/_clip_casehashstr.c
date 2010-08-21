CLIP_DLLEXPORT clip_hash_t
_clip_casehashstr(const char *x)
{
   return _clip_casehashbytes(0, x, strlen(x));
}
