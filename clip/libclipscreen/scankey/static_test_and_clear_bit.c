static int
test_and_clear_bit(int bit, void *addr)
{
   int b = bit % BITS_PER_LONG;

   unsigned long *wp = ((unsigned long *) addr) + bit / BITS_PER_LONG;

   int r = *wp & (1 << b);

   *wp &= ~(1 << b);
   return r;
}
