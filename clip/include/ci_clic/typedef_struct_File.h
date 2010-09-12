typedef struct File
{
   char *name_of_File;
   char *cname_of_File;
   char *s_cname_of_File;
   char *mname_of_File;
   char *origname_of_File;

   Coll Coll_functions_of_File;
   Coll Coll_unsortedFunctions_of_File;
   Coll Coll_externFunctions_of_File;
   Coll Coll_undeclExternFunctions_of_File;
   Coll Coll_externModules_of_File;

   Coll Coll_numbers_of_File;
   Coll Coll_unsortedNumbers_of_File;
   Coll Coll_strings_of_File;
   Coll Coll_unsortedStrings_of_File;

   Coll Coll_codeblocks_of_File;

   Function *Function_main_of_File;
   Function *Function_init_of_File;
   Node *Node_ctext_of_File;
   Node *Node_ctextStatic_of_File;
   int seq_no_of_File;
   int staticNo_of_File;
   int pubcount_of_File;
   int allcount_of_File;
   int initcount_of_File;
   int exitcount_of_File;

   Coll Coll_names_of_File;

   int haveNil_of_File;
   int haveTrue_of_File;
   int haveFalse_of_File;
}
File;
