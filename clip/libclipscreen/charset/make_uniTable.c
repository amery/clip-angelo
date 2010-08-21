int
make_uniTable(cons_CharsetEntry * cp, int len, unsigned long buf[256])
{
   int i;

   for (i = 0; i < 256; i++)
		buf[i] = i < len ? cp[i].unich_of_cons_CharsetEntry : (unsigned long) i;

   return 0;
}
