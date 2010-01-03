int
main(int argc, char **argv)
{
   int /*ch, */ i, ii, ret, err_flag = 0;

   clock_t   beg, end, Beg, End;

   int       files = 0;

   struct tms ts;

   char      buf[256];

   char     *e;

   prgname = strrchr(argv[0], '/');

   if (prgname)
      prgname++;
   else
      prgname = argv[0];

   e = getenv("CLIP_HOSTCS");
   if (e && *e)
    {
       sourceCharset = targetCharset = strdup(e);
    }
   else if (!e)
    {
       e = getenv("CLIP_LANG");
       if (e == NULL)
	  e = getenv("LANG");
       if (!e || !*e || !strcmp(e, "C"))
	  e = getenv("LC_MESSAGES");
       if (!e || !*e || !strcmp(e, "C"))
	  e = getenv("LC_ALL");
       if (e && *e)
	{
	   char     *s = strrchr(e, '.');

	   if (s)
	    {
	       snprintf(buf, sizeof(buf), "%s", s + 1);
	       for (s = buf; *s; s++)
		  *s = tolower(*s);
	       sourceCharset = targetCharset = strdup(buf);
	    }
	}
    }

   {
      e = getenv("CLIP_LANG");
      if (e == NULL)
	 e = getenv("LANG");
      if (!e || !*e || !strcmp(e, "C"))
	 e = getenv("LC_MESSAGES");
      if (!e || !*e || !strcmp(e, "C"))
	 e = getenv("LC_ALL");
      if (e && *e)
       {
	  char     *s = strrchr(e, '.');

	  if (s)
	   {
	      snprintf(buf, sizeof(buf), "%s", s + 1);
	      for (s = buf; *s; s++)
		 *s = tolower(*s);
	      out_charset = strdup(buf);
	   }
       }
   }

   {
      char     *locale;

      locale = getenv("CLIP_LANG");
      if (!locale || !*locale)
	 locale = getenv("LANG");
     /*if (locale && *locale &&
        strcasecmp(locale, "C") && strcasecmp(locale, "POSIX")) */
     /*
        setlocale(LC_ALL, locale);
      */
   }

   if (!sourceCharset)
      sourceCharset = targetCharset = strdup("c");

   getEnvironment();

   init_Coll(&includePaths, NULL, NULL);
   init_Coll(&lib_dirs, NULL, NULL);
   init_Coll(&arglibs, NULL, NULL);

#if 1
   insert_Coll(&includePaths, ".");
   snprintf(buf, sizeof(buf), "%s/include", CLIPROOT);
   insert_Coll(&includePaths, strdup(buf));
#ifdef STD_LIBDIR
   snprintf(buf, sizeof(buf), STD_LIB_DIR);
   insert_Coll(&lib_dirs, strdup(buf));
#endif
   snprintf(buf, sizeof(buf), "%s/lib", CLIPROOT);
   insert_Coll(&lib_dirs, strdup(buf));
#endif
   init_Coll(&predefines, NULL, NULL);
   init_Coll(&poName, NULL, NULL);
   init_Coll(&paName, NULL, NULL);
   init_Coll(&include_files, NULL, NULL);

   snprintf(buf, sizeof(buf), "__CLIP__=\"%s\"", CLIP_VERSION);
   append_Coll(&predefines, strdup(buf));

   init_module();

   {
      char      buf[256], *s;

      s = getenv("HOME");
      if (s && *s)
       {
	  snprintf(buf, sizeof(buf), "%s/.cliprc", s);
	  getrc(buf);
       }

   }

   getrc(".cliprc");

   {
      char      buf[256], *s;

      DIR      *dp;

      s = CLIPROOT;
      if (s && *s)
       {
	  snprintf(buf, sizeof(buf), "%s/.cliprc", s);
	  getrc(buf);
       }

      snprintf(buf, sizeof(buf), "%s/cliprc", CLIPROOT);
      dp = opendir(buf);
      if (dp)
       {
	  struct dirent *ep;

	  struct stat st;

	  Coll      files;

	  int       i;

	  init_Coll(&files, free, strcmp);
	  while ((ep = readdir(dp)))
	   {
	      snprintf(buf, sizeof(buf), "%s/cliprc/%s", CLIPROOT, ep->d_name);
	      if (stat(buf, &st))
		 continue;
	      if (!S_ISREG(st.st_mode))
		 continue;
	      if (access(buf, R_OK))
		 continue;
	      insert_Coll(&files, strdup(buf));
	   }
	  closedir(dp);

	  for (i = 0; i < files.count; i++)
	   {
	      char     *name = (char *) files.items[i];

	      getrc(name);
	   }

	  destroy_Coll(&files);
       }
   }

   argc--;
   argv++;
   get_opt(argc, argv);

   argc -= optind;
   argv += optind;

   if (err_flag)
      return 1;

#if 0
   insert_Coll(&includePaths, ".");
   snprintf(buf, sizeof(buf), "%s/include", CLIPROOT);
   insert_Coll(&includePaths, strdup(buf));
#ifdef STD_LIBDIR
   snprintf(buf, sizeof(buf), STD_LIBDIR);
   insert_Coll(&lib_dirs, strdup(buf));
#endif
   snprintf(buf, sizeof(buf), "%s/lib", CLIPROOT);
   insert_Coll(&lib_dirs, strdup(buf));
#endif

   if (syntax_tree_flag)
    {
       write_obj_flag = 0;
       codegen_flag = 1;
       compile_flag = 0;
       pcode_flag = 0;
       pc_flag = 0;
       asm_flag = 0;
       exec_flag = 0;
    }

   if (!write_obj_flag)
    {
      /*codegen_flag = 0; */
       compile_flag = 0;
    }
   if (preproc_flag)
    {
       write_obj_flag = 0;
       codegen_flag = 0;
       syntax_tree_flag = 0;
       compile_flag = 0;
       exec_flag = 0;
       pcode_flag = 0;
       pc_flag = 0;
       asm_flag = 0;
       shared_flag = 0;
    }

   if (pcode_flag)
    {
       pc_flag = 0;
       asm_flag = 0;
       shared_flag = 0;
    }

   if (pc_flag)
    {
       pcode_flag = 1;
#ifdef USE_AS
       if (use_asm)
	  asm_flag = 1;
       else
	  asm_flag = 0;
#endif
    }

   if (xpc_flag)
    {
       pcode_flag = 1;
       pc_flag = 1;
#ifdef USE_AS
       if (use_asm)
	  asm_flag = 0;
       else
	  asm_flag = 1;
#endif
    }

#if 0
   if (shared_flag && pcode_flag)
    {
       v_printf(0, "conflict between -s and -p flags\n");
       exit(1);
    }
#endif

   if (pcode_flag && c_flag)
    {
       v_printf(0, "conflict between -c and -p flags\n");
       exit(1);
    }

  /*if ( exec_flag && !main_flag && !shared_flag)
     {
     v_printf(0, "-e(xec) flag without -M(ain) or -s(hared) flags\n");
     exit(2);
     } */

   if (pcode_flag)
    {
       compile_flag = 0;
    }

   if (nomain_flag && main_flag)
    {
       v_printf(0, "conflict between -n and -m flags\n");
       exit(1);
    }

   if (!exec_flag && oname)
    {
       char     *e;

       if (oname[0] == '/')
	  snprintf(buf, sizeof(buf), "%s", oname);
       else
	  snprintf(buf, sizeof(buf), "%s%s%s", outdir ? outdir : "", outdir ? "/" : "", oname);
       e = strrchr(buf, '/');
       if (e)
	{
	   *e = 0;
	   outdir = strdup(buf);
	}
    }

   if (!outdir)
      outdir = ".";

   if (outdir)
    {
       char      cdir[256];

       getcwd(cdir, sizeof(cdir));

       if (!chdir(outdir))
	{
	   getcwd(buf, sizeof(buf));
	   outdir = strdup(buf);
	   chdir(cdir);
	}
       else
	{
	   yyerror("cannot change to output dir '%s': %s", outdir, strerror(errno));
	   exit(1);
	}
    }

   if (!preproc_flag)
    {
       v_printf(2, "set source charset to %s\n", sourceCharset);
       v_printf(2, "set target charset to %s\n", targetCharset);
    }

   init_lex();
   init_parser();

   if (argc < 1)
      ii = -1;
   else
      ii = 0;

   Beg = times(&ts);
   if (argc > 0)
    {
       for (i = 0; i < argc; i++)
	{
	   char     *e;

	   e = argv[i];
	   if (e[0] == '-' && e[1] == 'L')
	    {
	       insert_Coll(&lib_dirs, strdup(e + 2));
	       continue;
	    }
	   e = strrchr(argv[i], '.');
	   if (!e)
	    {
	       e = argv[i];
	       if (e[0] == '-' && e[1] == 'l')
		 /*append_Coll(&arglibs, strdup(e+2)) */ ;
	       else
		  yyerror("unknown file type '' file '%s'", argv[i]);
	       continue;
	    }
	   else if (!strcasecmp(e, ".po"))
	      insert_Coll(&poName, strdup(argv[i]));
	   else if (!strcasecmp(e, ".pa"))
	      insert_Coll(&paName, strdup(argv[i]));
	   else if (strcasecmp(e, ".prg")
		    && strcasecmp(e, ".c")
		    && strcasecmp(e, ".cc")
		    && strcasecmp(e, OBJSUF) && strcasecmp(e, SOBJSUF) && strcasecmp(e, ".a") && strcasecmp(e, ".lib"))
	    {
	      /*yywarning("unknown file type '%s' file '%s'", e, argv[i]); */
	       continue;
	    }
	}
    }

   for (; clic_errorcount == 0 && ii < argc; ii++)
    {
       ++files;
       if (ii < 0)
	{
	   v_printf(1, "no input files, so use stdin; -h will help\n");
	   fflush(stderr);
	   set_locale_name("stdin");
	   ret = clic_parse("stdin", stdin);
	   add_name("stdin");
	}
       else
	{
	   char     *e;

	   e = strrchr(argv[ii], '.');
	   add_name(argv[ii]);

	   if (!e)
	      continue;
	   else if (!strcasecmp(e, ".c") || !strcasecmp(e, ".cc") || !strcasecmp(e, ".cpp"))
	    {
	       if (!preproc_flag)
		{
		   v_printf(1, "process file '%s' ..", argv[ii]);
		   v_neednl = 1;
		}

	       beg = times(&ts);
	       compile_CFile(argv[ii]);
	       end = times(&ts);

	       if (!preproc_flag)
		{
		   v_neednl = 0;
		   if (clic_errorcount == 0)
		      v_printf(1, ".. done, %s\n", diff_clock(beg, end));
		   else
		      pr_errorcount(1);
		}
	       continue;
	    }
	   else if (strcasecmp(e, ".prg"))
	    {
	       continue;
	    }

	   if (ii > 0)
	      main_flag = 0;

	   if (!preproc_flag)
	    {
	       v_printf(1, "parsing file '%s' ..", argv[ii]);
	       v_neednl = 1;
	    }
	   beg = times(&ts);
	   set_locale_name(argv[ii]);
	   ret = clic_parse(argv[ii], 0);
	   end = times(&ts);

	   if (!preproc_flag)
	    {
	       v_neednl = 0;
	       if (clic_errorcount == 0)
		  v_printf(1, ".. done (%d/%d %s, %d %s, %s)\n",
			   clic_line, all_lines, _clic_ngettext("line",
								"lines",
								clic_line),
			   clic_warncount, _clic_ngettext("warning", "warnings", clic_warncount), diff_clock(beg, end));
	       else
		  vr_printf(1, "%d %s, %d %s\n",
			    clic_errorcount, _clic_ngettext("error", "errors",
							    clic_errorcount),
			    clic_warncount, _clic_ngettext("warning", "warnings", clic_warncount));
	    }
	}
       if (ret)
	  break;

       if (clic_errorcount == 0 && codegen_flag)
	{
	   v_printf(2, "codegen file '%s' ..", curFile->name_of_File);
	   v_neednl = 1;
	   beg = times(&ts);
	   codegen_File(curFile);
	   end = times(&ts);
	   v_neednl = 0;
	   if (clic_errorcount == 0)
	      v_printf(2, ".. done, %s\n", diff_clock(beg, end));
	   else
	      pr_errorcount(2);
	}
       if (clic_errorcount == 0 && syntax_tree_flag)
	{
	   print_File(curFile);
	}
       if (clic_errorcount == 0 && write_obj_flag)
	{
	   if (pcode_flag)
	    {
	       long      len;

	       v_printf(1, "writing file '%s' ..", curFile->s_cname_of_File);
	       v_neednl = 1;
	       beg = times(&ts);
	       write_OFile(curFile, &len);
	       write_names(curFile);
	       end = times(&ts);
	       v_neednl = 0;
	       if (clic_errorcount == 0)
		  v_printf(1, ".. done, %ld %s ,%s\n", len, _clic_ngettext("byte", "bytes", len), diff_clock(beg, end));
	       else
		  pr_errorcount(1);
	    }
	   else
	    {
	       v_printf(2, "writing file '%s' ..", curFile->s_cname_of_File);
	       v_neednl = 1;
	       write_File(curFile);
	       write_names(curFile);
	       v_neednl = 0;
	       if (clic_errorcount == 0)
		  v_printf(2, ".. done\n");
	       else
		  pr_errorcount(2);
	    }
	}

       if (clic_errorcount == 0 && (compile_flag || pc_flag))
	{
	   if (ii)
	      main_flag = 0;
	   v_printf(1, "compile file '%s' ..", curFile->s_cname_of_File);
	   v_neednl = 1;
	   beg = times(&ts);
	   compile_File(curFile->cname_of_File);
	   end = times(&ts);
	   v_neednl = 0;
	   if (clic_errorcount == 0)
	      v_printf(1, ".. done, %s\n", diff_clock(beg, end));
	   else
	      pr_errorcount(1);

	   if (clic_errorcount == 0 && shared_flag && !exec_flag)
	    {
	       v_printf(1, "make shared object '%s' ..", curFile->s_cname_of_File);
	       v_neednl = 1;
	       beg = times(&ts);
	       share_File(curFile->cname_of_File);
	       end = times(&ts);
	       v_neednl = 0;
	       if (clic_errorcount == 0)
		  v_printf(1, ".. done, %s\n", diff_clock(beg, end));
	       else
		  pr_errorcount(1);

	    }
	}

       if (ii < 0)
	  break;

       delete_File(curFile);
       curFile = NULL;
    }

   if (clic_errorcount == 0 && exec_flag)
    {
       char      cmd[1024 * 8], *e;

       char      cfuncname[256], ofuncname[256];

       char     *libroot;

       int       i;

       Coll      ex, nm;

       init_Coll(&ex, free, strcasecmp);
       init_Coll(&nm, free, strcasecmp);
#ifdef STD_LIBDIR
       libroot = 0;
#else
       libroot = CLIPROOT;
#endif

       ++files;
#ifdef STD_LIBDIR
       if (eshared_flag || shared_flag)
	{
	   snprintf(cmd, sizeof(cmd), "-lclip");
	   add_name(cmd);
	}
       else
#endif
	{
	   e = (eshared_flag || shared_flag) ? CLIPSLIB : CLIPLIB;
	   lib_name(cmd, sizeof(cmd), libroot, "lib", e, strlen(e));
	   add_name(cmd);
	}
       for (e = CLIPLIBS; *e;)
	{
	   int       l;

	   l = strspn(e, " \t");
	   e += l;
	   l = strcspn(e, " \t");
	   if (!l)
	      break;
	   lib_name(cmd, sizeof(cmd), libroot, "lib", e, l);
	   add_name(cmd);
	   e += l;
	}
       for (e = ADDLIBS; *e;)
	{
	   int       l;

	   l = strspn(e, " \t");
	   e += l;
	   l = strcspn(e, " \t");
	   if (!l)
	      break;
	   memcpy(cmd, e, l);
	   cmd[l] = 0;
	   add_name(cmd);
	   e += l;
	}
       add_name(MATHLIB);
       add_name(DLLIB);

      /* generate _cfunctions */
       if (asm_flag)
	  sprintf(cfuncname, "%s_ref.s", oname);
       else
	  sprintf(cfuncname, "%s_ref.c", oname);
       sprintf(ofuncname, "%s_ref.o", oname);
       v_printf(1, "generate reference file '%s' ..", cfuncname);
       v_neednl = 1;
       beg = times(&ts);
       write_Cfunc(cfuncname, onum, ovect, &ex, &nm);
       check_names(&ex, &nm);
       end = times(&ts);
       v_neednl = 0;
       if (clic_errorcount == 0)
	  v_printf(1, ".. done, %s\n", diff_clock(beg, end));
       else
	  pr_errorcount(1);
       if (clic_errorcount)
	  goto end;

       v_printf(1, "compile file '%s' ..", cfuncname);
       v_neednl = 1;
       beg = times(&ts);
       compile_File(cfuncname);
       end = times(&ts);
       v_neednl = 0;
       if (clic_errorcount == 0)
	  v_printf(1, ".. done, %s\n", diff_clock(beg, end));
       else
	  pr_errorcount(1);

#ifdef USE_LD
       if (use_asm && (shared_flag || eshared_flag))
	{
	   int       ll;

	   const char *ld_prg, *ld_end;

	   if (shared_flag || eshared_flag)
	    {
	       ld_prg = LD_PRG;
	       ld_end = LD_END;
	    }
	   else
	    {
	       ld_prg = LDS_PRG;
	       ld_end = LDS_END;
	    }

	   snprintf(cmd, sizeof(cmd), "%s", ld_prg);
	   ll = strlen(cmd);
	   for (e = cmd + ll, i = 0; i < lib_dirs.count; ++i)
	    {
	       snprintf(e, sizeof(cmd) - ll, " -L%s", (char *) lib_dirs.items[i]);
	       ll = strlen(cmd);
	       e = cmd + ll;
	    }
	   ll = strlen(cmd);
	   snprintf(cmd + ll, sizeof(cmd) - ll, " %s %s %s -o %s",
		    optLevel ? COPT : "", genDebug ? CDBG : "", ofuncname, oname);
	   ll = strlen(cmd);
	   for (e = cmd + ll, i = 0; i < onum; ++i)
	    {
	       snprintf(e, sizeof(cmd) - ll, " %s", ovect[i]);
	       ll = strlen(cmd);
	       e = cmd + ll;
	    }
	   ll = strlen(cmd);
	   snprintf(cmd + ll, sizeof(cmd) - ll, " %s", ld_end);
	}
       else
#endif
	{
	   sprintf(cmd, "%s", CC);
	   for (e = cmd + strlen(cmd), i = 0; i < includePaths.count; ++i)
	    {
	       sprintf(e, " %s %s", INCLUDE_FLAG, (char *) includePaths.items[i]);
	       e = cmd + strlen(cmd);
	    }

	   for (e = cmd + strlen(cmd), i = 0; i < lib_dirs.count; ++i)
	    {
	       sprintf(e, " -L%s", (char *) lib_dirs.items[i]);
	       e = cmd + strlen(cmd);
	    }

	   sprintf(cmd + strlen(cmd), " %s %s %s %s %s %s %s",
		   optLevel ? COPT : "", genDebug ? CDBG : "", CFLAGS, ADDCFLAGS, ofuncname, OUT_FLAG, oname);
	   for (e = cmd + strlen(cmd), i = 0; i < onum; ++i)
	    {
	       sprintf(e, " %s", ovect[i]);
	       e = cmd + strlen(cmd);
	    }
	}

       v_printf(1, "make file '%s' ..", oname);
       v_neednl = 1;
       beg = times(&ts);
       v_printf(2, "%s\n", cmd);
       if (system(cmd))
	  yyerror("C level error in command: %s", cmd);
       else if (rmc_flag)
	{
	   unlink(cfuncname);
	   unlink(ofuncname);
	}

       end = times(&ts);
       v_neednl = 0;
       if (clic_errorcount == 0)
	  v_printf(1, ".. done, %s\n", diff_clock(beg, end));
       else
	  pr_errorcount(1);
    }

 end:
   End = times(&ts);

   resume_parser();
   resume_lex();
   resume_locale();

   if (!preproc_flag)
      v_printf(1, "clip: %d %s, %s\n", files, _clic_ngettext("file", "files", files), diff_clock(Beg, End));
   return clic_errorcount == 0 ? 0 : 1;
}
