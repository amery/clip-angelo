void
codegen_File(File * file)
{
   int i, j;

	for (i = 0; i < file->Coll_unsortedFunctions_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[i]);
      codegen_Function(fp);
   }

   file->Function_init_of_File = new_Function(strdup("_init"), new_VarColl(), 0, 0, 0, 0);
   file->Function_init_of_File->Node_body_of_Function = new_Node();
   /*join_Node(file->Function_init_of_File->Node_body_of_Function, file->Function_main_of_File->Node_staticInit_of_Function); */
	for (i = 0; i < file->Coll_unsortedFunctions_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[i]);
      traverse_Node(fp->Node_staticInit_of_Function, add_goto_trap, file->Function_init_of_File);
      join_Node(file->Function_init_of_File->Node_body_of_Function, fp->Node_staticInit_of_Function);
   }

   codegen_Function(file->Function_init_of_File);

	for (i = 0; i < file->Coll_codeblocks_of_File.count_of_Coll; i++)
   {
		VAR(Function, fp, file->Coll_codeblocks_of_File.items_of_Coll[i]);
      codegen_Function(fp);
   }

   i = 0;
   file->pubcount_of_File = 0;
   file->initcount_of_File = 0;
   file->exitcount_of_File = 0;
	file->allcount_of_File = file->Coll_functions_of_File.count_of_Coll + file->Coll_codeblocks_of_File.count_of_Coll;
	for (j = 0; j < file->Coll_unsortedFunctions_of_File.count_of_Coll; ++j)
   {
		VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[j]);
      switch (fp->isPublic_of_Function)
      {
      case 1:
	 fp->mno_of_Function = i++;
	 file->pubcount_of_File++;
	 break;
      case 2:
	 fp->mno_of_Function = i++;
	 file->initcount_of_File++;
	 break;
      case 3:
	 fp->mno_of_Function = i++;
	 ++file->exitcount_of_File;
	 break;
      }
   }
      for (j = 0; j < file->Coll_unsortedFunctions_of_File.count_of_Coll; ++j)
   {
		VAR(Function, fp, file->Coll_unsortedFunctions_of_File.items_of_Coll[j]);
      if (!fp->isPublic_of_Function)
	 fp->mno_of_Function = i++;
   }
      for (j = 0; j < file->Coll_codeblocks_of_File.count_of_Coll; ++j)
   {
		VAR(Function, fp, file->Coll_codeblocks_of_File.items_of_Coll[j]);
      fp->mno_of_Function = i++;
   }

}
