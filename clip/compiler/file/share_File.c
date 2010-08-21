void
share_File(const char *cname)
{
   char cmd[1024], oname[256], soname[256], *e;

   strcpy(oname, cname);
   e = strrchr(oname, '.');
   strcpy(e, OBJSUF);

   strcpy(soname, cname);
   e = strrchr(soname, '.');
   strcpy(e, SOBJSUF);

   sprintf(cmd, "%s", CC);
/*
   for (e = cmd + strlen(cmd), i = 0; i < includePaths.count; ++i)
   {
   sprintf(e, " %s %s", INCLUDE_FLAG, (char *) includePaths.items[i]);
   e = cmd + strlen(cmd);
   }
 */
   e = cmd + strlen(cmd);
   sprintf(e, " %s %s %s %s %s %s %s", optLevel ? COPT : "", genDebug ? CDBG : "", CFLAGS, SFLAGS, oname, OUT_FLAG, soname);

   v_printf(2, "%s\n", cmd);
   if (system(cmd))
      yyerror("C level error in command: %s", cmd);
}
