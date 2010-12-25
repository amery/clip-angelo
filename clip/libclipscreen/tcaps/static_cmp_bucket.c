static int
cmp_bucket(const void *p1, const void *p2)
{
   return strcmp((const char *) p1, ((const TermcapBucket *) p2)->name);
}
