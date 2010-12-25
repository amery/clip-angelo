static int
cmp_OffsEntry(void *p1, void *p2)
{
   OffsEntry *o1 = (OffsEntry *) p1;

   OffsEntry *o2 = (OffsEntry *) p2;

	if (o1->hash_of_OffsEntry < o2->hash_of_OffsEntry)
      return -1;
	else if (o1->hash_of_OffsEntry > o2->hash_of_OffsEntry)
      return 1;
   else
      return 0;
}
