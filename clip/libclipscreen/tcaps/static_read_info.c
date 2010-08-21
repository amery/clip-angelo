static int
read_info(Terminfo * info, char *filename)
{
   int name_size, bool_count, num_count, str_count, str_size;

   int i, fd, numread, size, r;

   char buf[MAX_ENTRY_SIZE], *str_table;

   if ((fd = open(filename, O_RDONLY
#ifdef _WIN32
		  | O_BINARY
#endif
	)) < 0)
      return -1;

   /* grab the header */
   if ((r = read(fd, buf, 12)) < 12)
      goto clret;

   if (LOW_MSB(buf) != MAGIC)
   {
    clret:
      close(fd);
      return (0);
   }
   name_size = LOW_MSB(buf + 2);
   bool_count = LOW_MSB(buf + 4);
   num_count = LOW_MSB(buf + 6);
   str_count = LOW_MSB(buf + 8);
   str_size = LOW_MSB(buf + 10);

   /* grab the name */
   size = M_MIN(MAX_NAME_SIZE, (unsigned) name_size);
   if (read(fd, buf, size) < size)
      goto clret;
   buf[MAX_NAME_SIZE] = '\0';

#if 0
   ptr->term_names = calloc(strlen(buf) + 1, sizeof(char));

   strcpy(ptr->term_names, buf);
#endif

   if (name_size > MAX_NAME_SIZE)
      lseek(fd, (off_t) (name_size - MAX_NAME_SIZE), 1);

   /* grab the booleans */
   size = M_MIN(BOOLCOUNT, (unsigned) bool_count);
   if (read(fd, buf, size) < size)
      goto clret;
   if (bool_count > BOOLCOUNT)
      lseek(fd, (off_t) (bool_count - BOOLCOUNT), 1);
   else
      for (i = bool_count; i < BOOLCOUNT; i++)
	 buf[i] = 0;

   for (i = 0; i < BOOLCOUNT; ++i)
   {
      int no = tnums.bools[i];

      if (no < 0)
	 continue;
      if (buf[i])
	 info->bools[no] = 1;
   }

   /*
    * If booleans end on an odd byte, skip it.  The machine they
    * originally wrote terminfo on must have been a 16-bit
    * word-oriented machine that would trap out if you tried a
    * word access off a 2-byte boundary.
    */
   if ((name_size + bool_count) % 2 != 0)
      read(fd, buf, 1);

   /* grab the numbers */
   size = M_MIN(NUMCOUNT * 2, (unsigned) num_count * 2);
   if (read(fd, buf, size) < size)
      goto clret;

   for (i = 0; i < M_MIN(num_count, NUMCOUNT); i++)
   {
      int no = tnums.nums[i];

      if (no < 0)
	 continue;

      if (info->nums[no] != -1)
	 continue;

      if (IS_NEG1(buf + 2 * i))
	 continue;
      else if (IS_NEG2(buf + 2 * i))
	 continue;
      else
	 info->nums[no] = LOW_MSB(buf + 2 * i);
   }
   if (num_count > NUMCOUNT)
      lseek(fd, (off_t) (2 * (num_count - NUMCOUNT)), 1);

   if (str_count && str_size)
   {
      /* grab the string offsets */
      numread = read(fd, buf, (unsigned) (str_count * 2));
      if (numread < str_count * 2)
	 goto clret;

      str_table = buf + numread;
      if (str_count > STRCOUNT)
	 lseek(fd, (off_t) (2 * (str_count - STRCOUNT)), 1);

      /* finally, grab the string table itself */
      numread = read(fd, str_table, (unsigned) str_size);
      if (numread != str_size)
	 goto clret;

      for (i = 0; i < str_count; i++)
      {
	 int no = tnums.strs[i].num;

	 int type = tnums.strs[i].type;

	 if (no < 0)
	    continue;

	 if (type == 'S' && info->strings[no] != -1)
	    continue;
	 if (type == 'K' && info->keys[no] != -1)
	    continue;

	 if (IS_NEG1(buf + 2 * i))
	    continue;
	 else if (IS_NEG2(buf + 2 * i))
	    continue;
	 else
	 {
	    int offs, l;

	    char *s, *p;

	    offs = LOW_MSB(buf + 2 * i);
	    s = str_table + offs;
	    l = strlen(s) + 1;
	    place_buf(info, l);

#if 0
	    memcpy(info->buf + info->bufpos, s, l);
#else
	    for (l = 0, p = info->buf + info->bufpos; *s;)
	    {
	       int c;

	       /* skip the delay info */
	       if ((c = s[0]) == '$' && s[1] == '<')
	       {
		  char *s1;

		  for (s1 = s + 2; *s1; ++s1)
		     if (*s1 == '>')
			break;
		  if (*s1)
		     s = s1 + 1;
		  else
		     s = s1;
	       }
	       else
	       {
		  *p = c;
		  ++s;
		  ++p;
		  ++l;
	       }
	    }
	    *p = 0;
	    ++l;
#endif

	    if (type == 'S')
	       info->strings[no] = info->bufpos;
	    else if (type == 'K')
	       info->keys[no] = info->bufpos;
	    info->bufpos += l;
	 }
      }
   }

   return 0;
}
