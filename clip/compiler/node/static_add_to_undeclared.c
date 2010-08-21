static int
add_to_undeclared(char *name, Function ** fp)
{
   int no, fno;

   Function buf;

   *fp = 0;
   buf.name_of_Function = name;
   buf.isPublic_of_Function = 1;
   if (search_Coll(&curFile->Coll_functions_of_File, &buf, &fno))
   {
      if (!search_Coll(&curFile->Coll_externFunctions_of_File, name, &no))
      {
			*fp = (Function *) curFile->Coll_functions_of_File.items_of_Coll[fno];
	 if ((*fp)->isPublic_of_Function)
	    return 1;
	 else
	    return 2;
      }
   }
   else
   {
      if (!search_Coll(&curFile->Coll_undeclExternFunctions_of_File, name, &no))
      {
	 char *s = strdup(name);

	 char *p;

	 for (p = s; *p; p++)
	    *p = toupper(*p);
	 insert_Coll(&curFile->Coll_undeclExternFunctions_of_File, s);
      }
   }

   return 1;
}
