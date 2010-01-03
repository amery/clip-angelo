static void
write_names(File * file)
{
   char      path[256];

   char     *e;

   int       l;

   FILE     *out;

   int       i;

   if (!names_flag)
      return;

   snprintf(path, sizeof(path), "%s", file->cname_of_File);
   e = strrchr(path, '.');
   if (e)
      l = e - path;
   else
      l = strlen(path);

   snprintf(path + l, sizeof(path) - l, ".nm");

   out = fopen(path, "wt");
   if (out)
    {
       for (i = 0; i < file->Coll_functions_of_File.count; i++)
	{
	   Function *fp = (Function *) file->Coll_functions_of_File.items[i];

		if (fp->isPublic_of_Function != 1)
	      continue;
		fprintf(out, "%s\n", fp->name_of_Function);
	}
       fclose(out);
    }
   else
      yywarning("cannot open defname file '%s': %s", path, strerror(errno));

   snprintf(path + l, sizeof(path) - l, ".ex");

   out = fopen(path, "wt");
   if (out)
    {
       for (i = 0; i < file->Coll_undeclExternFunctions_of_File.count; i++)
	{
	   char     *s = (char *) file->Coll_undeclExternFunctions_of_File.items[i];

	   fprintf(out, "%s\n", s);
	}
       fclose(out);
    }
   else
      yywarning("cannot open exname file '%s': %s", path, strerror(errno));

}
