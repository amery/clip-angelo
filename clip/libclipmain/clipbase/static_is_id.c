static int
is_id(int c)
{
   if (isalnum(c))
      return 1;
   if (c == '_')
      return 1;
   return 0;
}
