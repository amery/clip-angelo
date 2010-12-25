static void
squeesh_entry(char *entry)
{
   int i, l = strlen(entry);

   char *s = entry;

   for (i = 0; i < l - 2;)
   {
      if (s[i] == '%' && s[i + 1] == 'p' && isdigit(s[i + 2]))
      {
	 memmove(s + i, s + i + 3, l - i - 2);
	 l -= 3;
      }
      else
	 i++;
   }
}
