static int
refcmp(const void *p1, const void *p2)
{
   if (p1 < p2)
      return -1;
   else if (p1 > p2)
      return 1;
   else
      return 0;
}
