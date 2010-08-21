int
clip_TRUENAME(ClipMachine * ClipMachineMemory)	/* Standardizes the path designation */
{
   char *dname = NULL, *dsk = NULL, buf[MAXPATHLEN], s[MAXPATHLEN];

   int len = 0, i = 0, n = 0, np = 0;

   if (ClipMachineMemory->argc < 1)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
		/*_clip_trap_printf( ClipMachineMemory, __FILE__, __LINE__, "invalid argument" );*/
      return 1;
   }
   dname = _clip_parc(ClipMachineMemory, 1);
   if (strlen(dname) > 1 && dname[1] == ':')
   {
      dsk = dname;
      strcpy(s, dname);
   }
   else
   {
      dsk = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);
      if (*dname == '\\')
      {
	 s[0] = 0;
      }
      else
      {
	 char *dir = _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[*dsk - 65]);

	 for (i = 0, n = 0; dir[i]; i++, n++)
	    s[n] = (dir[i] == '/' ? '\\' : dir[i]);
	 s[n] = 0;
	 if (dir[1] != 0)
	    strcat(s, "\\");
      }
      strcat(s, dname);
   }
   len = strlen(s);

   for (i = 0, n = 0; i < len; i++)
   {
      switch (s[i])
      {
      case '\\':
	 if (i > 0 && s[i - 1] == '\\')
	    continue;
	 switch (np)
	 {
	 case 1:
	    n--;
	    np = 0;
	    continue;
	 case 2:
	    for (n -= 4; n > 0; n--)
	    {
	       if (buf[n - 1] == '\\')
		  break;
	    }
	    if (n <= 0)
	       n = 1;
	    np = 0;
	    continue;
	 }
	 break;
      case '.':
	 if (np > 0 || buf[n - 1] == '\\')
	    np++;
	 break;
      default:
	 np = 0;
      }
      buf[n++] = s[i];
   }
   if (np == 1)
   {
      n -= 2;
   }
   else if (np == 2)
   {
      for (n -= 4; n > 0; n--)
      {
	 if (buf[n - 1] == '\\')
	    break;
      }
   }
   if (n <= 0)
      n = 1;
   if (n > 1 && buf[n - 1] == '\\')
      n--;

   buf[n++] = 0;

   if (n < 2 || buf[1] != ':')
   {
      memmove(buf + 2, buf, n);
      memcpy(buf, dsk, 2);
   }

   _clip_retc(ClipMachineMemory, buf);
   return 0;
}
