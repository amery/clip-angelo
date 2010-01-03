void
write_OFile(File * file, long *len)
{
   int       i, j, count;

   FILE     *out;

   time_t    timestamp;

   StrBuf   *bp;

   char     *s;

   long      modbeg;

   long      modlen;

   long      loffs, funcOffs;

   long      strOffs, numOffs, initOffs;

   long      soffs;

   char     *hash_buckets_buf = 0;

   int       hash_buckets_len = 0;

  /*printf("write_OFile: %s\n", file->name_of_File);fflush(stdout); */

/* generate hash-buckets string */
   {
      Coll      coll;

      int       sum = 0;

      int       count = file->Coll_names_of_File.count;
      StrBuf    str_buf = { 0, 0, 0, 0 };
      StrBuf   *hbp = &str_buf;

      int       l;

      int       nstatics = 0;

      for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
       {
	  VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	  nstatics += fp->VarColl_statics_of_Function->coll.count;
       }

      init_Coll(&coll, free, cmp_hash_bucket);
      hbp = new_StrBuf();

      for (i = 0; i < count; ++i)
       {
	  char     *s = (char *) file->Coll_names_of_File.items[i];

	  ClipHashBucket *buck = NEW(ClipHashBucket);

	  buck->hash_of_ClipHashBucket = hashstr(s);
	  buck->offs_of_ClipHashBucket = sum;
	  insert_Coll(&coll, buck);
	  l = strlen(s) + 1;
	  sum += l;
       }
      putLong_StrBuf(hbp, count);
      putLong_StrBuf(hbp, nstatics);

      for (i = 0; i < file->Coll_unsortedFunctions_of_File.count; i++)
       {
	  VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
	  for (j = 0; j < fp->VarColl_statics_of_Function->coll.count; j++)
	   {
			VAR(Var, vp, fp->VarColl_statics_of_Function->coll.items[j]);
	      putLong_StrBuf(hbp, hashstr(vp->name));
	   }
       }

      for (i = 0; i < count; ++i)
       {
	  ClipHashBucket *buck = (ClipHashBucket *) coll.items[i];

	  putLong_StrBuf(hbp, buck->hash_of_ClipHashBucket);
	  putLong_StrBuf(hbp, buck->offs_of_ClipHashBucket);
       }
      for (i = 0; i < count; ++i)
       {
	  char     *s = (char *) file->Coll_names_of_File.items[i];

	  putStr_StrBuf(hbp, s);
       }

      putByte_StrBuf(hbp, 0);
		hash_buckets_len = hbp->ptr_of_StrBuf - hbp->buf_of_StrBuf - 1;
		hash_buckets_buf = hbp->buf_of_StrBuf;

      destroy_Coll(&coll);
   }

   time(&timestamp);
   bp = new_StrBuf();
   *len = 0;

   if (getCount_Node(file->Node_ctext_of_File) || getCount_Node(file->Node_ctextStatic_of_File))
    {
       yyerror("cannot write PCODE module with c-texts");
       goto _ret;
    }

   out = fopen(file->cname_of_File, "wb");
   if (!out)
    {
       yyerror("cannot open output file '%s'", file->cname_of_File);
       goto _ret;
    }

   if (pc_flag)
    {
       fprintf(out, "/*\n");
       fprintf(out, " *\tfile '%s', \n", file->cname_of_File);
       fprintf(out, " *\tautomatically generated by clip\n");
       fprintf(out, " *\tfrom source %s\n", file->origname_of_File);
       fprintf(out, " *\tat %s", ctime(&timestamp));
       fprintf(out, " */\n\n");

#ifdef USE_AS
       if (asm_flag)
	{
	   fprintf(out, "\t.file\t\"%s\"\n\n", file->cname_of_File);
	}
       else
#endif
	{
	   fprintf(out, "#include \"ci_clip.h\"\n\n");
	}

    }

  /* magic */
   write_StrBuf(bp, "!<pobj>\n", 8);
  /* 8 */
   putLong_StrBuf(bp, timestamp);
   modlen = CUROFFS;
  /* 8, 1l */
   putLong_StrBuf(bp, 0);
   modbeg = CUROFFS;
  /* 8,2l == modbeg */
   putLong_StrBuf(bp, 0);	/* static Offs */
  /* 8, 3l */
   putLong_StrBuf(bp, file->staticNo_of_File + 1);	/* 1 for init flag */
  /* 8,4l */
   putShort_StrBuf(bp, file->Coll_unsortedNumbers_of_File.count);
  /* 8,4l,1s */
   putShort_StrBuf(bp, file->Coll_unsortedStrings_of_File.count + 1);
  /* 8,4l,2s */
   putLong_StrBuf(bp, file->pubcount_of_File);
  /* 8,5l,2s */
   putLong_StrBuf(bp, file->allcount_of_File);
  /* 8,6l,2s */
   numOffs = CUROFFS;
   putShort_StrBuf(bp, 0);
  /* 8,6l,3s */
   strOffs = CUROFFS;
   putShort_StrBuf(bp, 0);
  /* 8,6l,4s */
   initOffs = CUROFFS;
   putLong_StrBuf(bp, 0);
  /* 8, 7l, 4s */
   funcOffs = CUROFFS;
   putLong_StrBuf(bp, 0);
  /* 8, 8l, 4s */
/* add */
   putShort_StrBuf(bp, file->initcount_of_File);
   putShort_StrBuf(bp, file->exitcount_of_File);
/* eadd */
  /* 8, 8l, 6s */
   putStr_StrBuf(bp, file->origname_of_File);
   SETSHORT(bp, numOffs, BEGOFFS);
   for (j = 0; j < file->Coll_unsortedNumbers_of_File.count; j++)
    {
       VAR(ConstNode, sp, file->Coll_unsortedNumbers_of_File.items[j]);
       putDouble_StrBuf(bp, strtod(sp->val, NULL));
    }
   for (j = 0; j < file->Coll_unsortedNumbers_of_File.count; j++)
    {
       VAR(ConstNode, sp, file->Coll_unsortedNumbers_of_File.items[j]);
       int       dec = 0;

       char     *s = strchr(sp->val, '.');

       int       len = strlen(sp->val);

       if (s)
	  dec = len - (s - sp->val) - 1;
       if (len < 10)
	{
	   if (dec > 0)
	      len = 11 + dec;
	   else
	      len = 10;
	}
      /*if (dec < 2)
         dec = 2; */
       if (dec > len)
	  len = dec + 1;
       putByte_StrBuf(bp, len);
    }
   for (j = 0; j < file->Coll_unsortedNumbers_of_File.count; j++)
    {
       VAR(ConstNode, sp, file->Coll_unsortedNumbers_of_File.items[j]);
       int       dec = 0;

       char     *s = strchr(sp->val, '.');

       int       len = strlen(sp->val);

       if (s)
	  dec = len - (s - sp->val) - 1;
       if (len < 10)
	  len = 10;
      /*if (dec < 2)
         dec = 2; */
       if (dec > len)
	  len = dec + 1;
       putByte_StrBuf(bp, dec);
    }

   count = file->Coll_unsortedStrings_of_File.count;
   SETSHORT(bp, strOffs, BEGOFFS);
   soffs = CUROFFS;
   for (j = 0; j < count + 1; j++)
    {
       putLong_StrBuf(bp, 0);
       putLong_StrBuf(bp, 0);
    }
   for (j = 0; j < count; j++)
    {
       VAR(ConstNode, sp, file->Coll_unsortedStrings_of_File.items[j]);
       int       le = strlen(sp->val);

      /*int le = sp->len; */

       SETLONG(bp, soffs, le);
       LONGINCR(soffs);
       SETLONG(bp, soffs, BEGOFFS);
       LONGINCR(soffs);
       write_StrBuf(bp, sp->val, le + 1);
    }

   {
      SETLONG(bp, soffs, hash_buckets_len);
      LONGINCR(soffs);
      SETLONG(bp, soffs, BEGOFFS);
      LONGINCR(soffs);
      write_StrBuf(bp, hash_buckets_buf, hash_buckets_len + 1);
   }

   SETLONG(bp, initOffs, BEGOFFS);
   put_function(file, bp, file->Function_init_of_File, 0);
   loffs = CUROFFS;
   SETLONG(bp, funcOffs, BEGOFFS);
   for (j = 0; j < file->allcount_of_File; ++j)
    {
       putLong_StrBuf(bp, 0);	/* hash */
       putLong_StrBuf(bp, 0);	/* offs */
    }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
    {
       VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
		 if (fp->isPublic_of_Function == 1)
	  put_function(file, bp, fp, &loffs);
    }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
    {
       VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
		 if (fp->isPublic_of_Function == 2)
	  put_function(file, bp, fp, &loffs);
    }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
    {
       VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
		 if (fp->isPublic_of_Function == 3)
	  put_function(file, bp, fp, &loffs);
    }
   for (j = 0; j < file->Coll_unsortedFunctions_of_File.count; ++j)
    {
       VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[j]);
		 if (!fp->isPublic_of_Function)
	  put_function(file, bp, fp, &loffs);
    }
   for (j = 0; j < file->Coll_codeblocks_of_File.count; ++j)
    {
       VAR(Function, fp, file->Coll_codeblocks_of_File.items[j]);
       put_function(file, bp, fp, &loffs);
    }

   SETLONG(bp, modlen, BEGOFFS);

#ifdef USE_AS
   if (asm_flag)
    {
       char     *mp;

       long      j, modlen, size;

       char     *name = strdup(file->name_of_File), *upname;

       char     *s;

       s = strchr(name, '.');
       if (s)
	  *s = 0;

       upname = strdup(name);
       for (i = 0; i < strlen(name); ++i)
	  upname[i] = toupper(name[i]);

       fprintf(out, ".data\n");
       fprintf(out, "\t.align 4\n");
      /*fprintf(out, "\t.type %s_statics,@object\n", name); */
       fprintf(out, "%s_statics:\n", name);

       for (i = 0; i < file->staticNo_of_File + 1; ++i)
	{
	   fprintf(out, ".byte 0x0\n");
	   fprintf(out, ".byte 0x0\n");
	   fprintf(out, "\t.zero 2\n");
	   fprintf(out, "\t.zero 12\n");
	}
      /*              fprintf(out, "\t.size %s_statics,%d\n", name, (file->staticNo_of_File + 1) * sizeof(ClipVar)); */
       fprintf(out, ".section\t.rodata\n");
      /*fprintf(out, "\t.type %s_body,@object\n", name); */
       fprintf(out, "%s_body:\n", name);

       mp = bp->buf;
       modlen = bp->ptr - bp->buf;
       size = 0;

       for (j = 0; j < modlen; j++)
	  fprintf(out, ".byte %d\n", mp[j]);

/*		fprintf(out, "\t.size %s_body,%ld\n", name, modlen);*/

       fprintf(out, ".globl %sclip__PCODE_%s\n", US, upname);
       fprintf(out, ".data\n");
       fprintf(out, "\t.align 32\n");
      /*fprintf(out, "\t.type clip__PCODE_%s,@object\n", upname); */
/*		fprintf(out, "\t.size clip__PCODE_%s,%d\n", upname, sizeof(ClipFile));*/
       fprintf(out, US "clip__PCODE_%s:\n", upname);

       fprintf(out, "\t.long 1\n");
       fprintf(out, "\t.long %s_body\n", name);
       fprintf(out, "\t.long %s_body\n", name);
       fprintf(out, "\t.long %ld\n", size);
       fprintf(out, "\t.long 3\n");
       fprintf(out, "\t.long .LC0\n");
       fprintf(out, "\t.long %s_statics\n", name);
       fprintf(out, "\t.long 3\n");
       fprintf(out, "\t.long %d\n", file->staticNo_of_File);
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long -1\n");

       fprintf(out, ".section\t.rodata\n");
       fprintf(out, ".LC0:\n\t.string \"%s\"\n", name);

       fprintf(out, ".data\n");

       fprintf(out, "\t.align 4\n");
      /*fprintf(out, "\t.type %s_cpfiles,@object\n", upname); */
       fprintf(out, "%s_cpfiles:\n", upname);
       fprintf(out, "\t.long %sclip__PCODE_%s\n", US, upname);
       fprintf(out, "\t.long 0\n");
/*		fprintf(out, "\t.size %s_cpfiles,%d\n", upname, sizeof(ClipFile *) * 2);*/
/*		fprintf(out, ".globl %sclip__MODULE_%s\n", US, upname);*/
       fprintf(out, ".globl %sclip__MODULE_%s\n", US, file->mname_of_File);
       fprintf(out, "\t.align 32\n");
/*		fprintf(out, "\t.type clip__MODULE_%s,@object\n", upname);*/
/*		fprintf(out, "\t.size clip__MODULE_%s,%d\n", upname, sizeof(ClipModule));*/
/*		fprintf(out, US"clip__MODULE_%s:\n ", upname);      */
       fprintf(out, US "clip__MODULE_%s:\n ", file->mname_of_File);
       fprintf(out, "\t.long .LC0\n");
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long 0\n");

       fprintf(out, "\t.long %s_cpfiles\n", upname);
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long 0\n");
       fprintf(out, "\t.long 0\n");

       free(name);
       free(upname);
    }
   else
#endif
   if (pc_flag)
    {
       char     *mp;

       long      j, k, modlen, size;

       char     *name = strdup(file->name_of_File);

       char     *s;

       s = strchr(name, '.');
       if (s)
	  *s = 0;
       fprintf(out, "static ClipVar %s_statics[] =\n{\n", name);
       for (i = 0; i < file->staticNo_of_File + 1; ++i)
	  fprintf(out, "\t{{0, 0}},\n");
       fprintf(out, "\n};\n");
       fprintf(out, "/*body1 of module %s*/\n", name);
       fprintf(out, "\nstatic const char %s_body[]=\n{\n", name);

		 mp = bp->buf_of_StrBuf;
		 modlen = bp->ptr_of_StrBuf - bp->buf_of_StrBuf;
       size = 0;
       j = 0;

       while (j < modlen)
	{
	   fprintf(out, "\t");
	   for (k = 0; k < 32 && j < modlen; ++j, ++k, ++size)
	      fprintf(out, "%ld,", (long) mp[j]);
	   fprintf(out, "\n");
	}

       fprintf(out, "\n};\n");
       fprintf(out, "\nstruct ClipFile clip__PCODE_");
       for (i = 0; i < strlen(name); ++i)
	  fputc(toupper(name[i]), out);
       fprintf(out, " =\n{\n");
       fprintf(out, "\t1,\n");
       fprintf(out, "\t(char*)%s_body,\n", name);
       fprintf(out, "\t(char*)%s_body,\n", name);
       fprintf(out, "\t%ld,\n", size);
       fprintf(out, "\t3,\n");
       fprintf(out, "\t\"%s\",\n", /*arname */ name);
       fprintf(out, "\t%s_statics,\n", name);
       fprintf(out, "\t3,\n");
       fprintf(out, "\t%d,\n", file->staticNo_of_File);
       fprintf(out, "\t0,\n");
       fprintf(out, "\t0,\n");
       fprintf(out, "\t0,\n");
       fprintf(out, "\t0,\n");
       fprintf(out, "\t0,\n");
       fprintf(out, "\t-1,\n");
       fprintf(out, "};\n\n");

       for (i = 0; i < strlen(name); ++i)
	  name[i] = toupper(name[i]);

       fprintf(out, "static ClipFile *%s_cpfiles[]=\n{\n", name);
       fprintf(out, "\t&clip__PCODE_%s,\n", name);
       fprintf(out, "\t0\n};\n");

/*		fprintf(out, "\nClipModule clip__MODULE_%s =\n{\n ", name);*/
       fprintf(out, "\nClipModule clip__MODULE_%s =\n{\n ", file->mname_of_File);
       fprintf(out, "\t\"%s\",\n", name);
       fprintf(out, "\t0,\n");
       fprintf(out, "\t0,\n");
       fprintf(out, "\t0,\n");
      /*fprintf(out, "\t&clip__PCODE_%s,\n", name); */

       fprintf(out, "\t%s_cpfiles,\n", name);
       fprintf(out, "\t0,\n");
       fprintf(out, "\t0,\n");
       fprintf(out, "\t0,\n");
       fprintf(out, "};\n\n");

       free(name);
    }
   else
    {
		 for (s = bp->buf_of_StrBuf; s < bp->ptr_of_StrBuf; s += 4096)
	{
		int       size = 4096, rest = bp->ptr_of_StrBuf - s;

	   if (rest < 4096)
	      size = rest;
	   if (fwrite(s, size, 1, out) != 1)
	    {
	       yyerror("error writing file '%s', %s", file->cname_of_File, strerror(errno));
	       break;
	    }
	}
    }

   *len = ftell(out);
   fclose(out);
 _ret:
   delete_StrBuf(bp);
}