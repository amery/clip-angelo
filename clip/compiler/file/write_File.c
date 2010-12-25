void
write_File(File * file)
{
   int i, j, sum;

   FILE *out;

   time_t tbuf;

   out = fopen(file->cname_of_File, "wb");
   if (!out)
   {
      yyerror("cannot open output file '%s'", file->cname_of_File);
      return;
   }

   fprintf(out, "/*\n");
   fprintf(out, " *\tfile '%s', \n", file->cname_of_File);
   fprintf(out, " *\tautomatically generated by clip-");
   printVersion(out);
   fprintf(out, "\n");
   fprintf(out, " *\tfrom source '%s', \n", file->name_of_File);
   time(&tbuf);
   fprintf(out, " *\tat %s", ctime(&tbuf));
   fprintf(out, " */\n");

   fprintf(out, "\n#include \"ci_clip.h\"\n\n");

   fprintf(out, "static const char *_file = \"%s\";\n\n", file->name_of_File);

   fprintf(out, "/* extern functions */\n");
	for (i = 0; i < file->Coll_externFunctions_of_File.count_of_Coll; i++)
   {
		VAR(char, sp, file->Coll_externFunctions_of_File.items_of_Coll[i]);

      fprintf(out, "ClipFunction clip_%s;\n", sp);
   }

   fprintf(out, "\n/* forced references to external functions */\n");
	for (i = 0; i < file->Coll_externFunctions_of_File.count_of_Coll; i++)
   {
		VAR(char, sp, file->Coll_externFunctions_of_File.items_of_Coll[i]);

      fprintf(out, "static ClipFunction *__%s = clip_%s;\n", sp, sp);
   }

   fprintf(out, "\n/* forced references to external modules */\n");
	for (i = 0; i < file->Coll_externModules_of_File.count_of_Coll; i++)
   {
		VAR(char, sp, file->Coll_externModules_of_File.items_of_Coll[i]);

      fprintf(out, "extern ClipModule clip__MODULE_%s;\n", sp);
      fprintf(out, "static ClipModule *%s_module_ref = &%s_module;\n", sp, sp);
   }

#if 0
   fprintf(out, "\n/* implicit declared functions */\n");
   for (i = 0; i < file->Coll_undeclExternFunctions_of_File.count; i++)
   {
      VAR(char, sp, file->Coll_undeclExternFunctions_of_File.items[i]);

      fprintf(out, "ClipFunction %s;\n", sp);
   }
#endif

   fprintf(out, "\n/* file functions */\n");
   /*fprintf(out, "ClipFunction %s;\n", file->Function_main_of_File->name); */
	for (i = 0; i < file->Coll_unsortedFunctions_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[i]);
      fprintf(out, "%sClipFunction clip_%s;\n", fp->isPublic_of_Function ? "" : "static ", fp->name_of_Function);
   }

   fprintf(out, "\n/* entry table */\n");

   fprintf(out, "static ClipNameEntry __cfunctions[] =\n{\n");
	for (i = 0; i < file->Coll_functions_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_functions_of_File.items_of_Coll[i]);
      if (fp->isPublic_of_Function == 1)
	 fprintf(out, "\t{ %ld, clip_%s },\n", (long) hashstr(fp->name_of_Function), fp->name_of_Function);
   }
   fprintf(out, "\t{ 0, 0 }\n};\n\n");

   fprintf(out, "static ClipFunction *__inits[] =\n{\n");
	for (i = 0; i < file->Coll_functions_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_functions_of_File.items_of_Coll[i]);
      if (fp->isPublic_of_Function == 2)
	 fprintf(out, "\tclip_%s,\n", fp->name_of_Function);
   }
   fprintf(out, "\t0\n};\n\n");

   fprintf(out, "static ClipFunction *__exits[] =\n{\n");
	for (i = 0; i < file->Coll_functions_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_functions_of_File.items_of_Coll[i]);
      if (fp->isPublic_of_Function == 3)
	 fprintf(out, "\tclip_%s,\n", fp->name_of_Function);
   }
   fprintf(out, "\t0\n};\n\n");

   fprintf(out, "ClipModule clip__MODULE_%s=\n{\n", file->mname_of_File);

   fprintf(out, "\t\"%s\",\n", file->mname_of_File);
   fprintf(out, "\t__cfunctions,\n");
   fprintf(out, "\t__inits,\n");
   fprintf(out, "\t__exits,\n");
   fprintf(out, "\t0,\n");
   fprintf(out, "\t0,\n");
   fprintf(out, "\t0,\n");
   fprintf(out, "\t0,\n");

   fprintf(out, "};\n");

   /*if (genDebug) */
   {
      char buf[256];

      Coll coll;

      init_Coll(&coll, free, strcmp);

		fprintf(out, "\nstatic ClipHashBucket _hash_buckets[%d]=\n{\n", file->Coll_names_of_File.count_of_Coll);
		for (sum = 0, i = 0; i < file->Coll_names_of_File.count_of_Coll; ++i)
      {
	 int l;

	 char *s = (char *) file->Coll_names_of_File.items_of_Coll[i];

	 snprintf(buf, sizeof(buf), "\t{ %ld, %d }, /* %s */\n", (long) hashstr(s), sum, s);
	 insert_Coll(&coll, strdup(buf));
	 l = strlen(s) + 1;
	 sum += l;

      }

      for (i = 0; i < coll.count_of_Coll; ++i)
			fprintf(out, "%s", (char *) coll.items_of_Coll[i]);

      fprintf(out, "};\n");

      fprintf(out, "\nstatic char _hash_mem[%d] =\n{\n", sum);
		for (i = 0; i < file->Coll_names_of_File.count_of_Coll; ++i)
      {
	 int l, j;

	 char *s = (char *) file->Coll_names_of_File.items_of_Coll[i];

	 l = strlen(s) + 1;
	 fprintf(out, "\t");
	 for (j = 0; j < l; ++j)
	    fprintf(out, "%d, ", s[j]);
	 fprintf(out, "\n");
      }
      fprintf(out, "};\n");

      fprintf(out, "\nstatic ClipHashNames _hash_names =\n{\n");
		fprintf(out, "\t%d,\n", file->Coll_names_of_File.count_of_Coll);
      fprintf(out, "\t_hash_buckets,\n");
      fprintf(out, "\t_hash_mem,\n");
      fprintf(out, "};\n");

      destroy_Coll(&coll);
   }

   fprintf(out, "\n");

	for (j = 0; j < file->Coll_unsortedNumbers_of_File.count_of_Coll; j++)
   {
		VAR(ConstNode, sp, file->Coll_unsortedNumbers_of_File.items_of_Coll[j]);
      char *s = strchr(sp->val, '.');

      int len, dec = 0;

      len = strlen(sp->val);
      if (s)
	 dec = len - (s - sp->val) - 1;
      if (!s)
	 s = strchr(sp->val, 'e');
      if (!s)
	 s = strchr(sp->val, 'E');
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
      fprintf(out, "static ClipVarNum _num_%d = { { { NUMERIC_type_of_ClipVarType,%d,%d,0,0,0 } , %s%s } };\n", j, len, dec, sp->val, s ? "" : ".0");
   }

   for (j = 0; j < file->Coll_unsortedStrings_of_File.count_of_Coll; j++)
   {
		VAR(ConstNode, sp, file->Coll_unsortedStrings_of_File.items_of_Coll[j]);
      fprintf(out, "static ClipVarStr _str_%d = { { { CHARACTER_type_of_ClipVarType,%d,0,F_MSTAT_ClipFlags,0 }, { ", j, haveMacro(sp->val));
      printString(out, sp->val);
      fprintf(out, ", %ld } } };\n", (long) strlen(sp->val));
   }

   if (main_flag)
      write_Main(file, out, file->Function_main_of_File->name_of_Function);

   file->Node_ctext_of_File->pass(file->Node_ctext_of_File, CText, 0, out);

	if (file->Coll_codeblocks_of_File.count_of_Coll)
   {

      fprintf(out, "\n");
		for (j = 0; j < file->Coll_codeblocks_of_File.count_of_Coll; ++j)
      {
			VAR(Function, fp, file->Coll_codeblocks_of_File.items_of_Coll[j]);
	 fprintf(out, "static ClipFunction clip_%s;\n", fp->name_of_Function);
      }
   }

   fprintf(out, "\n/* file statics */\n");
   /*write_statics(out, file->Function_main_of_File->statics_of_Function); */
	for (i = 0; i < file->Coll_unsortedFunctions_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[i]);
      write_statics(out, fp->VarColl_statics_of_Function);
   }

   {
      int nstatics = 0;

      fprintf(out, "static ClipVarDef _statics[] =\n{\n");
		for (i = 0; i < file->Coll_unsortedFunctions_of_File.count_of_Coll; i++)
      {
			VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[i]);
			nstatics += fp->VarColl_statics_of_Function->coll.count_of_Coll;
      }
      fprintf(out, "\t{ %d, 0 },\n ", nstatics);
		for (i = 0; i < file->Coll_unsortedFunctions_of_File.count_of_Coll; i++)
      {
			VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[i]);
	 write_staticDefs(out, fp->VarColl_statics_of_Function);
      }
      fprintf(out, "};\n");
   }

   fprintf(out, "\nstatic int _init_flag=0;\n");
   write_Function(out, file, file->Function_init_of_File);

	if (file->Coll_codeblocks_of_File.count_of_Coll)
   {

      fprintf(out, "\n");

		for (j = 0; j < file->Coll_codeblocks_of_File.count_of_Coll; j++)
      {
			VAR(Function, fp, file->Coll_codeblocks_of_File.items_of_Coll[j]);
	 write_Function(out, file, fp);
      }

   }

/*write_Function(out, file, file->Function_main_of_File); */
for (i = 0; i < file->Coll_unsortedFunctions_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[i]);
      write_Function(out, file, fp);
   }

   fclose(out);
}
