static int
cmp_UniRecord(void *p1, void *p2)
{
   unsigned long l1, l2;

	l1 = ((UniRecord *) p1)->no_of_UniRecord;
	l2 = ((UniRecord *) p2)->no_of_UniRecord;

   if (l1 < l2)
      return -1;
   else if (l1 > l2)
      return 1;
   else
      return 0;
}
