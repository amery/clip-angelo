CLIP_DLLEXPORT clip_hash_t
_clip_hashstr(const char *x)
{
   return _clip_hashbytes(0, x, strlen(x));
}
