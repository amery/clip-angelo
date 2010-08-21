CLIP_DLLEXPORT clip_hash_t
_clip_casehashbytes(long seed, const char *bytes, int len)
{
   unsigned long crc32val;

   /* 0xXXXXXXXX handled specially */
   if (seed == 0 && len == 10 && bytes[0] == '0' && toupper(bytes[1]) == 'X'
       && is_hex(bytes[2]) && is_hex(bytes[3]) && is_hex(bytes[4]) && is_hex(bytes[5]) && is_hex(bytes[6]) && is_hex(bytes[7]) && is_hex(bytes[8]) && is_hex(bytes[9]))
   {
      crc32val = strtoul((const char *) bytes, 0, 16);
      return (clip_hash_t) crc32val;
   }
   return _clip_casehashbytes_(seed, (const char *) bytes, len);
}
