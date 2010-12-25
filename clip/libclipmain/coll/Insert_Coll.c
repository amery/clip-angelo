int
Insert_Coll(Coll * coll, void *item, int *index)
{
   int i;

   if (!coll->compare)
   {
      append_Coll(coll, item);
      return 1;
   }

if (search_Coll(coll, item, &i) == 0 || coll->duplicates_of_Coll)
   {
      atInsert_Coll(coll, item, i);
      if (index)
	 *index = i;
      return 1;
   }
   return 0;
}
