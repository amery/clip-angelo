static void
put_aqueue(long key)
{
   int r = shift_state;

   if (r & ((1 << KG_ALT) | (1 << KG_ALTGR)))
      key = META1(key);

   put_queue(key);
}
