CLIP_DLLEXPORT clip_hash_t
_clip_casehashword(const char *x, int l)
{
   const char *e;

   for (e = x + l; x < e && isspace(*x); x++)
      ;
   for (; e > x && isspace(e[-1]); e--)
      ;

   return _clip_casehashbytes(0, x, e - x);
}
