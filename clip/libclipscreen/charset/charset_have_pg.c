int
charset_have_pg(cons_CharsetEntry * cs, int len)
{
   int i;

   for (i = 0; i < len; i++, cs++)
   {
		if (cs->ch_of_cons_CharsetEntry > 128 && cs->ch_of_cons_CharsetEntry < 256 && cs->unich_of_cons_CharsetEntry >= 0x2500 && cs->unich_of_cons_CharsetEntry <= 0x25FF)
	 return 0;
   }

   return 1;
}
