static int
chg_bit(int bit, void *addr)
{
   int b = bit % BITS_PER_LONG;

   unsigned long *wp = ((unsigned long *) addr) + bit / BITS_PER_LONG;

   int r = *wp & (1 << b);

   if (r)
      *wp &= ~(1 << b);
   else
      *wp |= (1 << b);

   return !r;
}
