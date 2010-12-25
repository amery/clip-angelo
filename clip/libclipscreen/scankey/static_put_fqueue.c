static void
put_fqueue(long key)
{
   int r = shift_state;

   if (r & ((1 << KG_ALT) | (1 << KG_ALTGR)))
      key = META1(key);
   else if (r & ((1 << KG_CTRL) | (1 << KG_CTRLL) | (1 << KG_CTRLR)))
      key = META2(key);
   else if (r & ((1 << KG_SHIFT) | (1 << KG_SHIFTL) | (1 << KG_SHIFTR)))
      key = META1(META2(key));

   put_queue(key);
}
