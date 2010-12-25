void
remove_ClipVect(ClipVect * vect, int no)
{
   int count = vect->count_of_ClipVect;

   if (no < 0 || no >= count)
      return;
   vect->count_of_ClipVect = --count;
   memmove(vect->items_of_ClipVect + no, vect->items_of_ClipVect + no + 1, (count - no) * sizeof(void *));
}
