static int
is_dig(char *str)
{
   int c;

   while ((c = *str++))
   {
      if (!isdigit(c))
	 return 0;
   }
   return 1;
}
