static int
_cdx_cmp(RDD_ORDER * ro, unsigned char *s1, unsigned char *s2, int len)
{
   int r;

   if (ro->type == 'C' && !ro->binary)
   {
      r = loc_cmp(ro->index->loc, s1, s2, len);
   }
   else if (ro->type != 'X')
   {
      r = memcmp(s1, s2, ro->bufsize);
   }
   else
   {
      r = s1[0] - s2[0];
      if (r == 0)
      {
	 if (s1[0] == type_weight(CHARACTER_type_of_ClipVarType) && !ro->binary)
	 {
	    r = loc_cmp(ro->index->loc, s1 + 1, s2 + 1, len - 1);
	 }
	 else
	 {
	    r = memcmp(s1 + 1, s2 + 1, len - 1);
	 }
      }
   }
   return r;
}
