
static int
cmp_hash_bucket(const void *p1, const void *p2)
{
   unsigned long h1 = (unsigned long) GETLONG(p1);

   unsigned long h2 = ((ClipHashBucket *) p2)->hash_of_ClipHashBucket;

   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
   else
      return 0;
}
