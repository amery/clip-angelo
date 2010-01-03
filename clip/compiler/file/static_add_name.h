static void
add_name(Coll * cp, char *s)
{
   int       no;

   if (!names_flag)
      return;
   if (!search_Coll(cp, s, &no))
      atInsert_Coll(cp, strdup(s), no);
}
