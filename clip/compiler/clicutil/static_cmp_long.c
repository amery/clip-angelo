static int
cmp_long(void *p1, void *p2)
{
   long l1 = *(long *) p1;

   long l2 = *(long *) p2;

   if (l1 < l2)
      return -1;
   else if (l1 > l2)
      return 1;
   else
      return 0;
}
