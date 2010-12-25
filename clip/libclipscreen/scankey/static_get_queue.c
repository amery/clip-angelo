static long
get_queue(void)
{
   long r;

   if (!queue_beg)
      return 0;
   if (queue_end == queue_beg)
      return 0;
   r = *queue_beg;
   queue_end--;
   if (queue_beg != queue_end)
      memmove(queue_beg, queue_beg + 1, (queue_end - queue_beg) * sizeof(*queue_beg));
   return r;
}
