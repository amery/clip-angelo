CLIP_DLLEXPORT clip_hash_t
_clip_hashbytes(long seed, const char *bytes, int len)
{
   return _clip_hashbytes32(seed, bytes, len);
}
