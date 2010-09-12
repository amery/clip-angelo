static int
cmp_Label(void *p1, void *p2)
{
   Label *l1 = (Label *) p1;

   Label *l2 = (Label *) p2;

	if (l1->hash_of_Label < l2->hash_of_Label)
      return -1;
	else if (l1->hash_of_Label > l2->hash_of_Label)
      return 1;
   else
      return 0;
}
