void
print_File(File * file)
{
   int       i, count;

   printf("\nFile '%s' (%d functions):\n", file->name_of_File, file->Coll_functions_of_File.count);

   count = file->Coll_externFunctions_of_File.count;
   printf("external functions: %d:\n", count);
   for (i = 0; i < count; ++i)
    {
       VAR(char, sp, file->Coll_externFunctions_of_File.items[i]);

       printf("\t%s\n", sp);
    }

  /*
     count = file->Coll_undeclExternFunctions_of_File.count;
     printf( "implicit declared external functions: %d:\n", count);
     for (i = 0; i < count; ++i)
     {
     VAR(char, sp, file->Coll_undeclExternFunctions_of_File.items[i]);
     printf( "\t%s\n", sp);
     }
   */

   print_Constants(&file->Coll_unsortedNumbers_of_File, "numbers");
   print_Constants(&file->Coll_unsortedStrings_of_File, "strings");
   printf("codeblocks: %d\n", file->Coll_codeblocks_of_File.count);
   for (i = 0; i < file->Coll_codeblocks_of_File.count; ++i)
    {
       VAR(Function, fp, file->Coll_codeblocks_of_File.items[i]);
       print_Function(fp);
    }

  /*print_Function( file->main); */
   count = file->Coll_unsortedFunctions_of_File.count;
   for (i = 0; i < count; ++i)
    {
       VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items[i]);
       print_Function(fp);
    }

}
