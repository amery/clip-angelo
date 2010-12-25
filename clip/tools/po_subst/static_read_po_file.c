static int
read_po_file(char *filename)
{
   FILE *in;

   PoFile *fp;

   PoEntry *ep;

   int first = 1;

   in = fopen(filename, "r");
   if (!in)
      return errno;

   fp = (PoFile *) calloc(1, sizeof(PoFile));
	fp->filename_of_PoFile = strdup(filename);
	fp->charset_of_PoFile = "iso-8859-1";
	fp->iconv_t_cd_of_PoFile = (iconv_t) (-1);
	init_Coll(&fp->Coll_ids_of_PoFile, delete_PoEntry, cmp_PoEntry);

   while ((ep = po_read_entry(in)))
   {
      if (first && ep->msg && !ep->msg[0] && ep->translated)
      {
	 char *s;

	 first = 0;
	 s = strstr(ep->translated, "charset=");
	 if (s)
	 {
	    int l;

	    s += 8;
	    l = strcspn(s, "\r\n \t");
	    if (l)
	    {
			 fp->charset_of_PoFile = (char *) malloc(l + 1);
			 memcpy(fp->charset_of_PoFile, s, l);
			 fp->charset_of_PoFile[l] = 0;

			 debug(1, "found charset %s", fp->charset_of_PoFile);
	       if (encoding)
	       {
				 fp->iconv_t_cd_of_PoFile = iconv_open(encoding, fp->charset_of_PoFile);
				 if (fp->iconv_t_cd_of_PoFile == (iconv_t) (-1))
			  debug(0, "ERROR: cannot set output translation %s->utf-8", fp->charset_of_PoFile);
		  else
			  debug(1, "set output translation %s->%s", fp->charset_of_PoFile, encoding);
	       }
	    }
	 }
      }
      debug(2, "read entry %s:%s", ep->msg, ep->translated);
		insert_Coll(&fp->Coll_ids_of_PoFile, ep);
   }

   append_Coll(&po_files, fp);

   fclose(in);

   return 0;
}
