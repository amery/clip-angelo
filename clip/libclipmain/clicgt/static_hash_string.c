static unsigned long
hash_string(const char *str)
{
   unsigned long int hval, g;

   /* Compute the hash value for the given string.  */
   hval = 0;
   while (*str != '\0')
   {
      hval <<= 4;
      hval += (unsigned long) *str++;
      g = hval & ((unsigned long) 0xf << (HASHWORDBITS - 4));
      if (g != 0)
      {
	 hval ^= g >> (HASHWORDBITS - 8);
	 hval ^= g;
      }
   }
   return hval;
}
