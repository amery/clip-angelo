static char *
find_msg(char *id)
{
   PoEntry e;

   int i;

   e.msg = id;

	for (i = 0; i < po_files.count_of_Coll; i++)
   {
      int ind;

		PoFile *fp = (PoFile *) po_files.items_of_Coll[i];

		if (search_Coll(&fp->Coll_ids_of_PoFile, &e, &ind))
      {
	 PoEntry *ep;

	 ep = (PoEntry *) fp->Coll_ids_of_PoFile.items_of_Coll[ind];
	 if (ep->translated && *ep->translated)
	 {
		 if (fp->iconv_t_cd_of_PoFile == (iconv_t) (-1))
	       return ep->translated;
	    else
	    {
	       static char *obuf = 0;

	       static int obuflen = 0;

	       int il, ol, l;

	       char *is, *os;

	       il = strlen(ep->translated);
	       if (obuflen < il * 4)
	       {
		  obuflen = il * 4;
		  obuf = (char *) realloc(obuf, obuflen);
	       }

	       is = ep->translated;
	       os = obuf;
	       ol = obuflen;
#ifdef OS_CYGWIN
	       iconv(fp->iconv_t_cd_of_PoFile, (const char **) &is, &il, &os, &ol);
#else
	       iconv(fp->iconv_t_cd_of_PoFile, &is, (size_t *) (&il), &os, (size_t *) & (ol));
#endif
	       l = obuflen - ol;
	       obuf[l] = 0;
	       return obuf;
	    }
	 }
      }
   }

   return id;
}
