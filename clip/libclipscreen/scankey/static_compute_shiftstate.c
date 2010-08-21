/* called after returning from RAW mode or when changing consoles -
   recompute k_down[] and shift_state from key_down[] */
/* maybe called when keymap is undefined, so that shiftkey release is seen */
static void
compute_shiftstate(void)
{
   int i, j, k, sym, val;

   shift_state = 0;
   for (i = 0; i < SIZE(k_down); i++)
      k_down[i] = 0;

   for (i = 0; i < SIZE(key_down); i++)
      if (key_down[i])
      {				/* skip this word if not a single bit on */
	 k = i * BITS_PER_LONG;
	 for (j = 0; j < BITS_PER_LONG; j++, k++)
	    if (test_bit(k, key_down))
	    {
	       sym = plain_map[k];
	       if (KTYP(sym) == KT_SHIFT)
	       {
		  val = KVAL(sym);
		  if (val == KVAL(K_CAPSSHIFT))
		     val = KVAL(K_SHIFT);
		  k_down[val]++;
		  shift_state |= (1 << val);
	       }
	    }
      }
#ifdef DBG2
   printf("compute shift_state=%ld\r\n", shift_state);
#endif
}
