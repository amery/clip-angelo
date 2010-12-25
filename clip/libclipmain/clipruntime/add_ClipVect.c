void
add_ClipVect(ClipVect * vect, void *item)
{
   int count = ++(vect->count_of_ClipVect);

   vect->items_of_ClipVect = realloc(vect->items_of_ClipVect, sizeof(void *) * count);

   vect->items_of_ClipVect[count - 1] = item;
}
