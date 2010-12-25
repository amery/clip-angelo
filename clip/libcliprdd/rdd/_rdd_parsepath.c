int
_rdd_parsepath(ClipMachine * ClipMachineMemory, const char *toopen, const char *suff, char **path, char **name, int oper, const char *__PROC__)
{
   char p[PATH_MAX];

   char ret[PATH_MAX];

   char *c, *e;

   strncpy(p, toopen, sizeof(p) - 1);
   c = strrchr(p, '\\');
   e = strrchr(p, '/');
   e = max(c, e);
   if (!((c = strrchr(p, '.')) && (*(c + 1) != '/') && (*(c + 1) != '\\') && (c >= e)))
   {
      strncat(p, suff, sizeof(p) - strlen(p) - 1);
   }
   strncpy(ret, p, PATH_MAX);
   if ((ClipMachineMemory->flags & TRANSLATE_FLAG) && ((ret[0] == '\\')))
   {
      char *dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

      snprintf(p, PATH_MAX, "%c:%s", *dname, ret);
   }
   if (_clip_path(ClipMachineMemory, p, ret, sizeof(ret), oper == EG_CREATE))
      return rdd_err(ClipMachineMemory, oper, errno, __FILE__, __LINE__, __PROC__, toopen);
   *path = strdup(ret);
   if (name)
   {
      c = strrchr(ret, '/');
      if (!c)
	 c = strrchr(ret, '\\');
      e = strrchr(ret, '.');
      *e = 0;
      *name = strdup(c + 1);
   }
   return 0;
}
