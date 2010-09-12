typedef struct ClipInitStruct
{
   /*ClipNameEntry _cfunctions[]; */
   ClipFunction *(*_clip_builtin) (long hash);
   ClipFunction **_inits, ****_libinits;
   ClipFunction **_exits, ****_libexits;
   const char **_pfunctions_of_ClipInitStruct;;
   struct ClipFile **_cpfiles, ****_libcpfiles;
   struct ClipObjRtti **_objrtti_of_ClipInitStruct;;
   void **_dbdrivers_of_ClipInitStruct;;
   char *CLIP_HOSTCS;
}
ClipInitStruct;
