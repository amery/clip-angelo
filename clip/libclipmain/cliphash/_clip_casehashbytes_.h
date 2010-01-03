CLIP_DLLEXPORT clip_hash_t
_clip_casehashbytes_(long seed, const char *bytes, int len)
{
   unsigned int i;

   clip_hash_t ret;

   unsigned char *s = (unsigned char *) bytes;

#if 0
   crc32val = (unsigned long) seed;
   for (i = 0; i < len; i++)
    {
       crc32val = crc32_tab[(crc32val ^ ((unsigned char) toupper(s[i]))) & 0xff] ^ (crc32val >> 8);
    }
#else
   s = malloc(len);
   memcpy(s, bytes, len);
   for (i = 0; i < len; i++)
      s[i] = ((unsigned char) toupper(s[i]));
   ret = _clip_hashbytes32(seed, (const char *) s, len);
   free(s);
   return ret;
#endif
}
