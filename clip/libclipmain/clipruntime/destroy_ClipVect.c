void
destroy_ClipVect(ClipVect * vect)
{
   if (!vect)
      return;
   free(vect->items_of_ClipVect);
}
