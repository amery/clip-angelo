static clip_hash_t
n_hashstr(char *str)
{
   char *s;

   int is_dig = 1;

   for (s = str; *s; s++)
   {
      if (!isdigit(*s))
      {
	 is_dig = 0;
	 break;
      }
   }

   if (is_dig)
      return (clip_hash_t) atol(str);
   else
      return hashstr(str);
}
