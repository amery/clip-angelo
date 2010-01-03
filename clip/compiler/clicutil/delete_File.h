void
delete_File(File * m)
{
   if (!m)
      return;

   free(m->name_of_File);
   free(m->cname_of_File);

   destroy_Coll(&m->Coll_functions_of_File);
   destroy_Coll(&m->Coll_unsortedFunctions_of_File);
   destroy_Coll(&m->Coll_externFunctions_of_File);
   destroy_Coll(&m->Coll_externModules_of_File);
   destroy_Coll(&m->Coll_undeclExternFunctions_of_File);

   destroy_Coll(&m->Coll_numbers_of_File);
   destroy_Coll(&m->Coll_unsortedNumbers_of_File);
   destroy_Coll(&m->Coll_strings_of_File);
   destroy_Coll(&m->Coll_unsortedStrings_of_File);

   destroy_Coll(&m->Coll_codeblocks_of_File);
   destroy_Coll(&m->Coll_names_of_File);

   free(m);
}
