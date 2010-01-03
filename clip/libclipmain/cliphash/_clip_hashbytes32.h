CLIP_DLLEXPORT clip_hash_t
_clip_hashbytes32(long seed, const char *bytes, int len)
{
   unsigned int i;

   unsigned long crc32val;

   long      ret;

   unsigned char *s = (unsigned char *) bytes;

   crc32val = (unsigned long) seed;
   for (i = 0; i < len; i++)
      crc32val = crc32_tab[(crc32val ^ s[i]) & 0xff] ^ (crc32val >> 8);
//      printf("\nhash32=%s,%d,%ld,%ld,%d,",bytes,len,seed,(clip_hash_t)crc32val,crc32val> 0x7FFFFFFF);

   if (sizeof(long /*clip_hash_t */ ) <= 4)
      return (clip_hash_t) crc32val;
   ret = (long) crc32val;
   if (crc32val > 0x7FFFFFFFL)
      ret = ret - 0XFFFFFFFFL - 1;
//      printf("%ld,%ld,%ld\n",0xFFFFFFFFL,ret,(clip_hash_t)ret);
   return (clip_hash_t) ret;

}
