CLIP_DLLEXPORT clip_hash_t
_clip_hashbytes16(long seed, const char *bytes, int len)
{
   unsigned int i;

   unsigned short crc16val;

   unsigned char *s = (unsigned char *) bytes;

   crc16val = (unsigned short) seed;
   for (i = 0; i < len; i++)
      crc16val = (crc16val >> 8) ^ crc16_tab[(crc16val & 0xff) ^ s[i]];

   return (clip_hash_t) crc16val;
}
