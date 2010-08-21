static int
test_bit(int bit, void *addr)
{
   int b = bit % BITS_PER_LONG;

   unsigned long *wp = ((unsigned long *) addr) + bit / BITS_PER_LONG;

   return *wp & (1 << b);
}
