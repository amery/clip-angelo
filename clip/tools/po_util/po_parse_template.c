int
po_parse_template(FILE * in, char *filename, char *start, char *stop,
		  void *par, int (*entry_handler) (void *par, char *filename, int line, char *txt), int (*norm_handler) (void *par, char *txt, int len))
{
   int ch, s0, p0;

   int sl, pl, pos = 0;

   ExtractState state;

   int mline = 0;

   int line = 0;

   int ret = 0;

   Buf buf;

   memset(&buf, 0, sizeof(Buf));

   sl = strlen(start);
   pl = strlen(stop);
   s0 = start[0];
   p0 = stop[0];

	state = E_Norm_of_ExtractState;

   for (;;)
   {
      ch = fgetc(in);
      if (ch == EOF)
	 return ferror(in);
      if (ch == '\n')
	 ++line;
    again:
      switch (state)
      {
			case E_Norm_of_ExtractState:
	 if (ch == s0)
	 {
		 state = E_Start_of_ExtractState;
	    pos = 1;
	    continue;
	 }
	 else
	 {
	    if (norm_handler)
	    {
	       char b = ch;

	       ret = norm_handler(par, &b, 1);
	       if (ret)
		  goto end;
	    }
	 }
	 break;
			case E_Start_of_ExtractState:
	 if (pos == sl)
	 {
		 state = E_Msg_of_ExtractState;
	    pos = 0;
		 buf.end_of_Buf = 0;
	    mline = line;
	    goto again;
	 }
	 if (ch != start[pos++])
	 {
		 state = E_Norm_of_ExtractState;
	    if (norm_handler)
	    {
	       ret = norm_handler(par, start, pos - 1);
	       if (ret)
		  goto end;
	       goto again;
	    }
	    continue;
	 }
	 break;
			case E_Msg_of_ExtractState:
	 if (ch == p0)
	 {
		 state = E_Stop_of_ExtractState;
	    pos = 1;
	    continue;
	 }
	 put_Buf(&buf, ch);
	 break;
			case E_Stop_of_ExtractState:
	 if (pos == pl)
	 {
		 state = E_Norm_of_ExtractState;
	    put_Buf(&buf, 0);

	    if (entry_handler)
	    {
			 ret = entry_handler(par, filename, mline, buf.ptr_of_Buf);
	       if (ret)
		  goto end;
	    }

	    buf.end_of_Buf = 0;
	    goto again;
	 }
	 if (ch != stop[pos])
	 {
	    int i;

	    for (i = 0; i < pos; ++i)
	       put_Buf(&buf, stop[i]);

		 state = E_Msg_of_ExtractState;
	    goto again;
	 }
	 pos++;
	 break;
      }
   }
 end:
    free(buf.ptr_of_Buf);
   return ret;
}
