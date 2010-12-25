CLIP_DLLEXPORT int
_clip_translate_charset(char *p1, char *p2, unsigned char *str, unsigned char *ostr, int len)
{
   cons_CharsetEntry *cs1 = 0, *cs2 = 0;

   int len1 = 0, len2 = 0;

   int ind;

   TranslateTable stbl, *tp = 0;

   if (!p1 || !p2 || !str || !ostr)
      return EG_ARG;

   if (!strcasecmp(p1, p2))
   {
      memcpy(ostr, str, len);
      return 0;
   }

   stbl.src = p1;
   stbl.dst = p2;

   if (tr_charsets && search_Coll(tr_charsets, &stbl, &ind))
   {
		tp = (TranslateTable *) tr_charsets->items_of_Coll[ind];
      goto translate;
   }

   if (load_charset_name(p1, &cs1, &len1))
   {
      _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", p1, strerror(errno));
   }

   if (load_charset_name(p2, &cs2, &len2))
   {
      _clip_logg(2, "translate_charset: cannot load charset file '%s': %s", p2, strerror(errno));
   }

   if (cs1 && cs2)
   {
      int src_pg, dst_pg;

      tp = (TranslateTable *) calloc(1, sizeof(TranslateTable));
      tp->src = strdup(p1);
      tp->dst = strdup(p2);
      make_translation(cs1, len1, cs2, len2, tp->tbl);

      src_pg = !charset_have_pg(cs1, len1);
      dst_pg = !charset_have_pg(cs2, len2);
      if (src_pg && !dst_pg)
      {
	 unsigned char *pg;

	 pg = (unsigned char *) malloc(256);
	 load_charset_tables(p1, 0, 0, 0, 0, pg);

	 memmove(pg, pg + 128, 128);
	 tp->pg = (unsigned char *) realloc(pg, 128);
      }

      if (!tr_charsets)
	 tr_charsets = new_Coll(delete_TranslateTable, cmp_TranslateTable);
      insert_Coll(tr_charsets, tp);
   }

   free(cs1);
   free(cs2);

 translate:
   if (tp)
   {
      int i;

      int s, s1;

      for (i = 0; i < len; i++)
      {
	 s = str[i];
	 if ((s & 0x80) && tp->pg && (s1 = tp->pg[s & 0x7f]))
	    ostr[i] = pg2pgch(s1);
	 else
	    ostr[i] = tp->tbl[s];
      }
   }
   else
   {
      memcpy(ostr, str, len);
   }

   return 0;
}
