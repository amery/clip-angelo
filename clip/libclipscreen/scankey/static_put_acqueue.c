static void
put_acqueue(long key)
{
   int r = shift_state;

   if (r & ((1 << KG_ALT) | (1 << KG_ALTGR)))
      key = META1(key);
   else if (r & ((1 << KG_CTRL) | (1 << KG_CTRLL) | (1 << KG_CTRLR)))
      key = META2(key);

   put_queue(key);
}
