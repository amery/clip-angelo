typedef struct Function
{
   char *name_of_Function;
   VarColl *VarColl_params_of_Function;
   VarColl *VarColl_locals_of_Function;
   VarColl *VarColl_statics_of_Function;
   VarColl *VarColl_memvars_of_Function;
   VarColl *VarColl_fields_of_Function;
   VarColl *VarColl_fmemvars_of_Function;
   VarColl *VarColl_parameters_of_Function;
   VarColl *VarColl_privates_of_Function;
   VarColl *VarColl_publics_of_Function;

   struct File *File_of_Function;

   Node *Node_body_of_Function;
   Node *Node_staticInit_of_Function;
   int isPublic_of_Function,	/* 0-static; 1-public; 2-init; 3-exit */
    isProc_of_Function:1, isMain_of_Function:1, lastExec_of_Function:1;
   int maxdeep_of_Function;
   int deep_of_Function;
   int line_of_Function;
   int file_of_Function;
   int no_of_Function;
   int offs_of_Function;
   int goto_trap_of_Function;
   int mno_of_Function;
   int labelNo_of_Function;
   int inLoop_of_Function;
   int seqNo_of_Function;
   int allSeqNo_of_Function;
   int seqLevel_of_Function;
   int *seqStack_of_Function;
   int haveSelf_of_Function;

   struct Function *Function_upfunc_of_Function;
   int reflocals_of_Function;
   int nlocals_of_Function;
   int uplocals_of_Function;

   int haveIf_of_Function;
   int haveReturn_of_Function;
   int isCodeblock_of_Function;
   VarColl *VarColl_code_params_of_Function;

   char *typename_of_Function;
}
Function;
