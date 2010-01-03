static void
print_Function(Function * fp)
{
   printf("\n");
	if (!fp->isPublic_of_Function)
      printf("STATIC ");
	printf("%s %s\n", fp->isProc_of_Function ? "PROCEDURE" : "FUNCTION", fp->isPublic_of_Function ? fp->name_of_Function : fp->name_of_Function + 3);
	printf("\tAS %s\n", fp->typename_of_Function ? fp->typename_of_Function : "undef");
	printf("nlocals = %d reflocals = %d uplocals=%d\n", fp->nlocals_of_Function, fp->reflocals_of_Function, fp->uplocals_of_Function);
	print_VarColl(fp->VarColl_params_of_Function, "params");
	print_VarColl(fp->VarColl_locals_of_Function, "locals");
	print_VarColl(fp->VarColl_statics_of_Function, "statics");
	print_VarColl(fp->VarColl_memvars_of_Function, "memvars");
	print_VarColl(fp->VarColl_fields_of_Function, "fields");
	print_VarColl(fp->VarColl_fmemvars_of_Function, "fmemvars");
	print_VarColl(fp->VarColl_parameters_of_Function, "paramteres");
	print_VarColl(fp->VarColl_privates_of_Function, "privates");
	print_VarColl(fp->VarColl_publics_of_Function, "publics");

	if (fp->Node_body_of_Function)
    {
       printf("body:\n");
		 fp->Node_body_of_Function->pass(fp->Node_body_of_Function, Print, 0, NULL);
    }
}
