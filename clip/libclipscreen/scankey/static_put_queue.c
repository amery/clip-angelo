static void
put_queue(long key)
{
   if (queue_end == queue_max)
   {
      int l = queue_max - queue_beg;

      int p = queue_end - queue_beg;

      l = l * 2;
      if (l < 16)
	 l = 16;
      queue_beg = (long *) realloc(queue_beg, l * sizeof(long));

      queue_end = queue_beg + p;
      queue_max = queue_beg + l;
   }
   *queue_end = key;
   queue_end++;
}
