static void
puts_queue(char *str)
{
   unsigned char *b = (unsigned char *) str;

   for (; *b; b++)
      put_queue(*b);

}
