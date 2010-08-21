/*
 * We have a combining character DIACR here, followed by the character CH.
 * If the combination occurs in the table, return the corresponding value.
 * Otherwise, if CH is a space or equals DIACR, return DIACR.
 * Otherwise, conclude that DIACR was not combining after all,
 * queue it and return CH.
 */
static unsigned char
handle_diacr(unsigned char ch)
{
   int d = diacr;

   int i;

   diacr = 0;

   for (i = 0; i < accent_table_size; i++)
   {
      if (accent_table[i].diacr == d && accent_table[i].base == ch)
	 return accent_table[i].result;
   }

   if (ch == ' ' || ch == d)
      return d;

   put_queue(d);
   return ch;
}
