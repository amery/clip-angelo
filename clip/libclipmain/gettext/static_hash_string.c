/* Defines the so called `hashpjw' function by P.J. Weinberger
   [see Aho/Sethi/Ullman, COMPILERS: Principles, Techniques and Tools,
   1986, 1987 Bell Telephone Laboratories, Inc.]  */

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
