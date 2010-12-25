static void
squeesh_if_need(char *entry)
{
   static int need_squeesh = 0, first = 1;

   if (first)
   {
      /* check for old-style tgoto - it don't und. %p1 */
      char buf[32];

      scr_tgoto("%p1%d", 2, 10, buf, sizeof(buf));
      if (strcmp(buf, "10"))
	 need_squeesh = 1;
      first = 0;
   }
   if (need_squeesh)
      squeesh_entry(entry);
}
