static int
cmp_CSRecord(void *p1, void *p2)
{
   unsigned char l1, l2;

	l1 = ((UniRecord *) p1)->cp_of_UniRecord->ch_of_cons_CharsetEntry;
	l2 = ((UniRecord *) p2)->cp_of_UniRecord->ch_of_cons_CharsetEntry;

   if (l1 < l2)
      return -1;
   else if (l1 > l2)
      return 1;
   else
      return 0;
}
