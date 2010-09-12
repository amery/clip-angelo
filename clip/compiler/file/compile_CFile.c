void
compile_CFile(char *name)
{
   char cmd[1024], cname[256], oname[256], *e;

   int i;

   if (preproc_flag)
   {
      preprocCFile(name, NULL);
      return;
   }

   /*snprintf(cname, sizeof(cname), "/tmp/clip.XXXXXX");
      mktemp(cname);
      strcat(cname, ".c"); */
   //snprintf(cname, sizeof(cname), "/tmp/clipar.%lx%lx.pa", (long) getpid(), (long) random());
   snprintf(cname, sizeof(cname), "/tmp/clipar.%lx%lx.c", (long) getpid(), (long) random());

   preprocCFile(name, cname);
   if (clic_errorcount)
      return;

   strcpy(oname, name);
   e = strrchr(oname, '.');
   strcpy(e, OBJSUF);

   sprintf(cmd, "%s", CC);
	for (e = cmd + strlen(cmd), i = 0; i < includePaths.count_of_Coll; ++i)
   {
		sprintf(e, " %s %s", INCLUDE_FLAG, (char *) includePaths.items_of_Coll[i]);
      e = cmd + strlen(cmd);
   }
   sprintf(e, " %s %s %s %s %s %s %s", optLevel ? COPT : "", genDebug ? CDBG : "", CFLAGS, COMPILE_FLAG, cname, OUT_FLAG, oname);

   v_printf(2, "%s\n", cmd);
   if (system(cmd))
      yyerror("C level error in command: %s", cmd);

   unlink(cname);
}
