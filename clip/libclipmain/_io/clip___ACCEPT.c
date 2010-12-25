int
clip___ACCEPT(ClipMachine * ClipMachineMemory)
{
   int l, c = ClipMachineMemory->history.count_of_ClipVect;

   int ckey = 0;

   int pos, cpos, size = 16;

   unsigned char *s;

   char *prompt = _clip_parcl(ClipMachineMemory, 1, &l);

   int attr = _clip_colorSelect(ClipMachineMemory);

   int hpos = c;

   dialog_init(ClipMachineMemory);
   if (prompt)
   {
		/*_clip_fullscreen(ClipMachineMemory); */
      /*out_dev(ClipMachineMemory, "\n", 1, attr, 1); */
      out_dev(ClipMachineMemory, prompt, l, attr, 1);
      sync_mp(ClipMachineMemory);
   }

   s = (unsigned char *) malloc(size);

   s[0] = 0;

   for (pos = 0, cpos = 0;;)
   {
      int i;

      sync_mp(ClipMachineMemory);

      ckey = _clip_key(ClipMachineMemory, -1, _CLIP_INKEY_KEYBOARD);

      if (pos >= size - 1)
      {
	 size = size * 3 / 2;
	 s = (unsigned char *) realloc(s, size);
      }

      switch (ckey)
      {
      case 13:			/* Enter */
	 s[pos] = 0;
	 goto ret;
      case 27:			/* Esc */
	 s[0] = 0;
	 goto ret;
      case 5:			/* Up */
	 if (!hpos)
	    break;
	 hpos--;
       newstr:
	 {
	    char *sp = ClipMachineMemory->history.items_of_ClipVect[hpos];

	    int l = strlen(sp);

	    free(s);
	    size = l + 4;
	    s = (unsigned char *) malloc(size);
	    memcpy(s, sp, l);
	    s[l] = 0;

	    for (i = 0; i < cpos; ++i)
	       out_dev(ClipMachineMemory, "\b", 1, attr, 1);

	    out_dev(ClipMachineMemory, (char *) s, l, attr, 1);

	    for (i = l; i < pos; ++i)
	       out_dev(ClipMachineMemory, " ", 1, attr, 1);

	    for (; i > l; --i)
	       out_dev(ClipMachineMemory, "\b", 1, attr, 1);

	    pos = cpos = l;
	 }
	 break;
      case 24:			/* Down */
	 if (hpos >= c - 1)
	    break;
	 ++hpos;
	 goto newstr;
      case 21:			/* Ctrl-U */
	 for (i = 0; i < cpos; ++i)
	    out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	 for (i = 0; i < pos; ++i)
	    out_dev(ClipMachineMemory, " ", 1, attr, 1);
	 for (i = 0; i < pos; ++i)
	    out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	 cpos = pos = 0;
	 break;
      case 1:			/* Home */
	 for (i = 0; i < cpos; ++i)
	    out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	 cpos = 0;
	 break;
      case 6:			/* End */
	 if (cpos < pos)
	 {
	    out_dev(ClipMachineMemory, (char *) (s + cpos), pos - cpos, attr, 1);
	    cpos = pos;
	 }
	 break;
      case 19:			/* Left */
	 if (cpos > 0)
	 {
	    out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	    --cpos;
	 }
	 break;
      case 4:			/* Right */
	 if (cpos < pos)
	 {
	    out_dev(ClipMachineMemory, (char *) (s + cpos), 1, attr, 1);
	    ++cpos;
	 }
	 break;
      case 8:			/* BS */
	 if (cpos > 0)
	 {
	    --pos;
	    --cpos;
	    if (pos == cpos)
	       out_dev(ClipMachineMemory, "\b \b", 3, attr, 1);
	    else
	    {
	       int l = pos - cpos;

	       memmove(s + cpos, s + cpos + 1, l);
	       s[pos] = ' ';
	       out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	       out_dev(ClipMachineMemory, (char *) (s + cpos), l + 1, attr, 1);
	       for (i = 0; i <= l; ++i)
		  out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	    }
	 }
	 break;
      case 7:			/* Del */
	 if (cpos < pos)
	 {
	    --pos;
	    {
	       int l = pos - cpos;

	       memmove(s + cpos, s + cpos + 1, l);
	       s[pos] = ' ';
	       out_dev(ClipMachineMemory, (char *) (s + cpos), l + 1, attr, 1);
	       for (i = 0; i <= l; ++i)
		  out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	    }
	 }
	 break;
      default:
	 if (ckey >= 32 && ckey < 256)
	 {
	    unsigned char b;

	    b = ckey;
	    if (cpos == pos)
	    {
	       s[cpos++] = b;
	       pos = cpos;
	       out_dev(ClipMachineMemory, (char *) (&b), 1, attr, 1);
	    }
	    else
	    {
	       int l = pos - cpos;

	       memmove(s + cpos + 1, s + cpos, l);
	       s[cpos] = b;
	       out_dev(ClipMachineMemory, (char *) (&b), 1, attr, 1);
	       out_dev(ClipMachineMemory, (char *) (s + cpos + 1), l, attr, 1);
	       for (i = 0; i < l; ++i)
		  out_dev(ClipMachineMemory, "\b", 1, attr, 1);
	       ++cpos;
	       ++pos;
	    }
	 }
	 break;
      }
   }

 ret:
   _clip_retc(ClipMachineMemory, (char *) s);

   if (c == 0 || strcmp(ClipMachineMemory->history.items_of_ClipVect[c - 1], (char *) s))
      add_ClipVect(&ClipMachineMemory->history, s);
   else
      free(s);

   if (c >= CLIP_MAX_HISTORY)
   {
      free(ClipMachineMemory->history.items_of_ClipVect[0]);
      remove_ClipVect(&ClipMachineMemory->history, 0);
   }

   out_dev(ClipMachineMemory, "\n", 1, attr, 1);
   sync_mp(ClipMachineMemory);

   return 0;
}
