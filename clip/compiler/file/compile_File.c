void
compile_File(const char *cname)
{
   char cmd[1024], oname[256], *e, *s;

   int i;

   strcpy(oname, cname);
   s = strrchr(cname, '.');
   e = strrchr(oname, '.');
   strcpy(e, OBJSUF);

#ifdef USE_AS
   if (asm_flag && !strcmp(s, ".s"))
   {
      snprintf(cmd, sizeof(cmd), "%s -o %s %s", AS_PRG, oname, cname);
   }
   else
#endif
   {

      snprintf(cmd, sizeof(cmd), "%s", CC);
		for (e = cmd + strlen(cmd), i = 0; i < includePaths.count_of_Coll; ++i)
      {
			snprintf(e, sizeof(cmd) - (e - cmd), " %s %s", INCLUDE_FLAG, (char *) includePaths.items_of_Coll[i]);
	 e = cmd + strlen(cmd);
      }
      snprintf(e, sizeof(cmd) - (e - cmd), " %s %s %s %s %s %s %s %s", optLevel ? COPT : "", genDebug ? CDBG : "", CFLAGS, ADDCFLAGS, COMPILE_FLAG, cname, OUT_FLAG, oname);

   }

   v_printf(2, "%s\n", cmd);
   if (system(cmd))
      yyerror("C level error in command: %s", cmd);
   else if (rmc_flag)
      unlink(cname);
}
