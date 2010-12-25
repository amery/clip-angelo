typedef struct ClipModule
{
   const char *name_of_ClipModule;
   ClipNameEntry *ClipNameEntry_cfunctions_of_ClipModule;
   ClipFunction **ClipFunction_inits_of_ClipModule;
   ClipFunction **ClipFunction_exits_of_ClipModule;
   ClipFile **cpfiles_of_ClipModule;
   const char **pfunctions_of_ClipModule;
   ClipObjRtti **objrtti_of_ClipModule;
   struct DBFuncTable **dbdrivers_of_ClipModule;
}
ClipModule;
