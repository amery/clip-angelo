int
remove_Coll(Coll * coll, void *item)
{

   int index;

   if (indexOf_Coll(coll, item, &index))
   {
      atRemove_Coll(coll, index);
      return 1;
   }
   return 0;
}
