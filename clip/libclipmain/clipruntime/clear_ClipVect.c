void
clear_ClipVect(ClipVect * vect)
{
   vect->count_of_ClipVect = 0;
   free(vect->items_of_ClipVect);
   vect->items_of_ClipVect = 0;
   /*vect->items_of_ClipVect = realloc(vect->items_of_ClipVect, sizeof(void *) * 4); */
}
