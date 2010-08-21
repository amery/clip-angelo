void
insert_ClipVect(ClipVect * vect, void *item, int pos)
{
   int count = vect->count_of_ClipVect;

   if (pos < 0 || pos > count)
      return;
   vect->count_of_ClipVect = ++count;
   vect->items_of_ClipVect = realloc(vect->items_of_ClipVect, sizeof(void *) * count);
   memmove(vect->items_of_ClipVect + pos + 1, vect->items_of_ClipVect + pos, (count - pos - 1) * sizeof(void *));

   vect->items_of_ClipVect[pos] = item;
}
