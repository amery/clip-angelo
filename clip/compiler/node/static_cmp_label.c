static int
cmp_label(void *p1, void *p2)
{
	long h1 = ((CaseLabel *) p1)->hash_of_CaseLabel;

	long h2 = ((CaseLabel *) p2)->hash_of_CaseLabel;

   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
   else
      return 0;
}
