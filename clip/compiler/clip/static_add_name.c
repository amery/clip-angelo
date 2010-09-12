static void
add_name(const char *name)
{
   char buf[256], *e, *s;

   if (!name || !*name)
      return;

   strcpy(buf, name);
   e = strrchr(buf, '.');
   s = strrchr(buf, '/');
   if (e && (!s || e > s))
   {
      if (!strcasecmp(e, ".c") || !strcasecmp(e, ".cc") || !strcasecmp(e, ".prg"))
	 strcpy(e, OBJSUF);
      else if (!strcasecmp(e, OBJSUF) || !strcasecmp(e, SOBJSUF) || !strcasecmp(e, LIBSUF) || !strcasecmp(e, SLIBSUF))
	 ;
      else if (!strcasecmp(e, ".po") || !strcasecmp(e, ".pa"))
	 goto o_nam;
      else if (buf[0] == '-' && buf[1] == 'l')
	 ;
      else
	 return;
   }
   else
   {
      /*e = buf + strlen(buf);
         strcpy(e, OBJSUF); */
#ifndef STD_LIBDIR
      if (name[0] == '-' && name[1] == 'l')
      {
	 snprintf(buf, sizeof(buf), "%s/lib/lib%s%s", CLIPROOT, name + 2, static_flag ? LIBSUF : DLLSUFF);
	 if (access(buf, R_OK))
	    strncpy(buf, name, sizeof(buf));
      }
#endif
   }

   ovect = (char **) realloc(ovect, sizeof(char *) * (onum + 1));

   ovect[onum] = strdup(buf);
   ++onum;
 o_nam:
   if (!oname)
   {
      strcpy(buf, name);
      e = strrchr(buf, '.');
      s = strrchr(buf, '/');
      if (e && (!s || e > s))
	 *e = 0;
      else
	 e = buf + strlen(buf);
#if 0
      if (shared_flag && !exec_flag)
	 strcpy(e, SOBJSUF);
#endif
      oname = strdup(buf);
   }
}
