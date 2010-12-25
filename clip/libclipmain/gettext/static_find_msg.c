static char *
find_msg(Locale * lp, const char *msgid, int *lenp)
{
   size_t top, act = 0, bottom;

   /* Locate the MSGID and its translation.  */
   if (lp->hash_size > 2 && lp->hash_tab != NULL)
   {
      /* Use the hashing table.  */
      nls_uint32 len = strlen(msgid);

      nls_uint32 hash_val = hash_string(msgid);

      nls_uint32 idx = hash_val % lp->hash_size;

      nls_uint32 incr = 1 + (hash_val % (lp->hash_size - 2));

      nls_uint32 nstr = W(lp->must_swap, lp->hash_tab[idx]);

      if (nstr == 0)
	 /* Hash table entry is empty.  */
	 return NULL;

	 if (W(lp->must_swap, lp->orig_tab[nstr - 1].length_of_string_desc) >= len && strcmp(msgid, lp->data + W(lp->must_swap, lp->orig_tab[nstr - 1].offset_of_string_desc)) == 0)
      {
	 if (lenp)
		 *lenp = W(lp->must_swap, lp->trans_tab[nstr - 1].length_of_string_desc);
	 return (char *) lp->data + W(lp->must_swap, lp->trans_tab[nstr - 1].offset_of_string_desc);
      }

      while (1)
      {
	 if (idx >= lp->hash_size - incr)
	    idx -= lp->hash_size - incr;
	 else
	    idx += incr;

	 nstr = W(lp->must_swap, lp->hash_tab[idx]);
	 if (nstr == 0)
	    /* Hash table entry is empty.  */
	    return NULL;

	 if (W(lp->must_swap, lp->orig_tab[nstr - 1].length_of_string_desc) >= len && strcmp(msgid, lp->data + W(lp->must_swap, lp->orig_tab[nstr - 1].offset_of_string_desc)) == 0)
	 {
	    if (lenp)
			 *lenp = W(lp->must_swap, lp->trans_tab[nstr - 1].length_of_string_desc);
		 return (char *) lp->data + W(lp->must_swap, lp->trans_tab[nstr - 1].offset_of_string_desc);
	 }
      }
      /* NOTREACHED */
   }

   /* Now we try the default method:  binary search in the sorted
      array of messages.  */
   bottom = 0;
   top = lp->nstrings;
   while (bottom < top)
   {
      int cmp_val;

      act = (bottom + top) / 2;
		cmp_val = strcmp(msgid, lp->data + W(lp->must_swap, lp->orig_tab[act].offset_of_string_desc));
      if (cmp_val < 0)
	 top = act;
      else if (cmp_val > 0)
	 bottom = act + 1;
      else
	 break;
   }

   if (lenp && bottom < top)
		*lenp = W(lp->must_swap, lp->trans_tab[act].length_of_string_desc);
   /* If an translation is found return this.  */
	return bottom >= top ? NULL : (char *) lp->data + W(lp->must_swap, lp->trans_tab[act].offset_of_string_desc);
}
