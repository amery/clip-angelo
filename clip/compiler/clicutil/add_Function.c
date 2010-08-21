void
add_Function(File * file, Function * func)
{
   int no;

   if (search_Coll(&file->Coll_functions_of_File, func, &no))
   {
		VAR(Function, fp, file->Coll_functions_of_File.items_of_Coll[no]);
      if (fp == mainFunction)
      {
	 char buf[80];

	 yywarning("declaration function with module name '%s'; force -n flag", fp->name_of_Function);
	 remove_Coll(&file->Coll_functions_of_File, fp);
	 snprintf(buf, sizeof(buf), "%s_m", fp->name_of_Function);
	 free(fp->name_of_Function);
	 fp->name_of_Function = strdup(buf);
	 insert_Coll(&file->Coll_functions_of_File, fp);

	 no = insert_Coll(&file->Coll_unsortedFunctions_of_File, func);
	 func->no_of_Function = no;
	 insert_Coll(&file->Coll_functions_of_File, func);

      }
      else
	 yyerror("function '%s' already defined near line %d file %s", fp->name_of_Function, fp->line_of_Function, fileName(fp->file_of_Function));
   }
   else if (!func->isPublic_of_Function && search_Coll(&file->Coll_externFunctions_of_File, func->name_of_Function, &no))
   {
      yyerror("static function '%s' already declared as external", func->name_of_Function);
   }
   else
   {
      no = insert_Coll(&file->Coll_unsortedFunctions_of_File, func);
      func->no_of_Function = no;
      insert_Coll(&file->Coll_functions_of_File, func);
   }
}
