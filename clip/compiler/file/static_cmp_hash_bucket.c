static int
cmp_hash_bucket(void *p1, void *p2)
{
   unsigned long h1 = (unsigned long) ((ClipHashBucket *) p1)->hash_of_ClipHashBucket;

   unsigned long h2 = (unsigned long) ((ClipHashBucket *) p2)->hash_of_ClipHashBucket;

   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
   else
      return 0;
}
