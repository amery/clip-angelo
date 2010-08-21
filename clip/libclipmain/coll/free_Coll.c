int
free_Coll(Coll * coll, void *item)
{
   int index;

   if (indexOf_Coll(coll, item, &index))
   {
      atFree_Coll(coll, index);
      return 1;
   }
   return 0;
}
