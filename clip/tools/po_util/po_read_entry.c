PoEntry *
po_read_entry(FILE * in)
{
   char buf[4096];

   PoEntry *ep = 0;

   Buf msgid, msgstr, *bp;

   Buf msgid_plural;

   int pl_index = -1;

   if (!fgets(buf, sizeof(buf), in))
      return 0;

   memset(&msgid, 0, sizeof(Buf));
   memset(&msgid_plural, 0, sizeof(Buf));
   memset(&msgstr, 0, sizeof(Buf));
   ep = new_PoEntry(0, 0, 0, 0);
   bp = 0;

   do
   {
      int l, r;

      char *p = buf;

      for (l = strlen(buf); l > 0 && (buf[l - 1] == '\n' || buf[l - 1] == '\r'); l--)
	 ;

      buf[l] = 0;

      if (l == 0)
      {
	 end_pl_index(ep, &pl_index, &msgstr);
	 break;
      }

      if (buf[0] == '#')
      {
	 int n = ep->ncomments++;

	 ep->comments = (char **) realloc(ep->comments, sizeof(char *) * (n + 1));
	 ep->comments[n] = strdup(buf);
	 continue;
      }

      if (l > 12 && !memcmp(buf, "msgid_plural", 12))
      {
	 end_pl_index(ep, &pl_index, &msgstr);
	 bp = &msgid_plural;
	 p += 12;
	 l -= 12;
      }
      else if (l > 5 && !memcmp(buf, "msgid", 5))
      {
	 end_pl_index(ep, &pl_index, &msgstr);
	 bp = &msgid;
	 p += 5;
	 l -= 5;
      }
      else if (l > 6 && !memcmp(buf, "msgstr", 6))
      {
	 end_pl_index(ep, &pl_index, &msgstr);
	 bp = &msgstr;
	 p += 6;
	 l -= 6;
	 if (p[0] == '[')
	 {
	    int ll;

	    p++;
	    l--;
	    ll = strcspn(p, "]");
	    pl_index = atoi(p);

	    p += ll;
	    l -= ll;

	    ll = strspn(p, "]");
	    p += ll;
	    l -= ll;
	 }
      }
      r = strspn(p, " \t");
      p += r;
      l -= r;

      if (*p == '"')
      {
			StrState state = S_norm_of_StrState;

	 char obuf[4];

	 if (!bp)
	    continue;

	 obuf[3] = 0;

	 for (p++, l--; l > 0; p++, l--)
	 {
	    int ch = *p;

	    switch (state)
	    {
			 case S_norm_of_StrState:
	       switch (ch)
	       {
	       case '\\':
				 state = S_bs_of_StrState;
		  break;
	       case '"':
				 state = S_stop_of_StrState;
		  break;
	       default:
		  put_Buf(bp, ch);
		  break;
	       }
	       break;
			 case S_bs_of_StrState:
			 state = S_norm_of_StrState;
	       switch (ch)
	       {
	       case 'n':
		  put_Buf(bp, '\n');
		  break;
	       case 'r':
		  put_Buf(bp, '\r');
		  break;
	       case 't':
		  put_Buf(bp, '\t');
		  break;
	       case 'v':
		  put_Buf(bp, '\v');
		  break;
	       case '0':
	       case '1':
	       case '2':
	       case '3':
	       case '4':
	       case '5':
	       case '6':
	       case '7':
				 state = S_oct1_of_StrState;
		  obuf[0] = ch;
		  break;
	       default:
		  put_Buf(bp, ch);
		  break;
	       }
	       break;
			 case S_oct1_of_StrState:
			 state = S_norm_of_StrState;
	       switch (ch)
	       {
	       case '0':
	       case '1':
	       case '2':
	       case '3':
	       case '4':
	       case '5':
	       case '6':
	       case '7':
				 state = S_oct2_of_StrState;
		  obuf[1] = ch;
		  break;
	       default:
		  put_Buf(bp, ch);
		  break;
	       }
	       break;
			 case S_oct2_of_StrState:
				 state = S_norm_of_StrState;
	       switch (ch)
	       {
	       case '0':
	       case '1':
	       case '2':
	       case '3':
	       case '4':
	       case '5':
	       case '6':
	       case '7':
		  obuf[2] = ch;
		  ch = strtol(obuf, 0, 8);
		  break;
	       default:
		  put_Buf(bp, ch);
		  break;
	       }
	       break;
			 case S_stop_of_StrState:
	       break;
	    }
	    	    if (state == S_stop_of_StrState)
	       break;
	 }

      }

   }
   while (fgets(buf, sizeof(buf), in));

   /*if(msgid.ptr) */
   put_Buf(&msgid, 0);
   /*if(msgstr.ptr) */
   put_Buf(&msgstr, 0);
	if (msgid_plural.ptr_of_Buf)
      put_Buf(&msgid_plural, 0);

	ep->msg = msgid.ptr_of_Buf;
	ep->msg_plural = msgid_plural.ptr_of_Buf;
	ep->translated = msgstr.ptr_of_Buf;

   return ep;
}
