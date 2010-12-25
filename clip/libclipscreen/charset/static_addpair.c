static void
addpair(int fp, unsigned long un, cons_CharsetEntry ** list, int *len)
{
   int pos = *len;

   (*len)++;
   *list = (cons_CharsetEntry *) realloc(*list, (*len) * sizeof(cons_CharsetEntry));
	(*list)[pos].ch_of_cons_CharsetEntry = fp;
	(*list)[pos].unich_of_cons_CharsetEntry = un;
}
