static void
split_entry(Terminfo * info, char *entry)
{
   char *bp;

   char name[3];

   name[2] = 0;
   bp = entry;
   while (bp && *bp)
   {
      TermcapBucket *fp;

      bp = tskip(bp);
      if (!bp[0] || !bp[1])
	 break;
      if (bp[0] == ':' || bp[1] == ':')
	 continue;
      name[0] = *bp++;
      name[1] = *bp++;

      /*if (*bp == '@')
         continue; */
      if (name[0] == ' ' || name[1] == ' ')
	 continue;

      fp = (TermcapBucket *) bsearch(name, termcapNames, TCAPNUM, sizeof(TermcapBucket), cmp_bucket);

      if (!fp)
      {
	 /*printf("unknown termcap entry: %s\n", name); */
	 continue;
      }

      switch (*bp)
      {
      case ':':
	 /* bool */
	 if (fp->type == 'B')
	 {
	    info->bools[fp->no] = 1;
	 }
	 break;
      case '#':
	 /* num */
	 if (fp->type == 'N')
	 {
	    int base, i;

	    ++bp;
	    if (info->nums[fp->no] != -1)
	       break;
	    base = 10;
	    if (*bp == '0')
	       base = 8;
	    i = 0;
	    while (*bp >= '0' && *bp <= '9')
	       i = i * base, i += *bp++ - '0';
	    info->nums[fp->no] = i;
	 }
	 break;
      case '=':
	 /* string */
	 {
	    int s;

	    ++bp;
	    if (fp->type == 'S')
	       if (info->strings[fp->no] != -1)
		  break;
	    if (fp->type == 'K')
	       if (info->keys[fp->no] != -1)
		  break;

	    s = tdecode(bp, info);
	    if (fp->type == 'S')
	       info->strings[fp->no] = s;
	    else if (fp->type == 'K')
	       info->keys[fp->no] = s;
	 }
	 break;
      case 0:
	 return;
      default:
	 continue;
      }
   }
}
