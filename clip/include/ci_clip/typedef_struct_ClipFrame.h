typedef struct ClipFrame
{
   ClipVar *ClipVar_stack_of_ClipFrame;
   ClipVar *ClipVar_sp_of_ClipFrame;
   const char *filename_of_ClipFrame;
   int line_of_ClipFrame;
   long *privates_of_ClipFrame;
   ClipVarFrame *ClipVarFrame_localvars_of_ClipFrame;
   ClipVarDef *ClipVarDef_locals_of_ClipFrame;
   ClipVarDef *ClipVarDef_statics_of_ClipFrame;
   struct ClipFrame *ClipFrame_up_of_ClipFrame;
   ClipHashNames *names_of_ClipFrame;
   const char *procname_of_ClipFrame;
   int stklen_of_ClipFrame;
   ClipVarVect *ClipVarVect_tempvars_of_ClipFrame;
}
ClipFrame;
