File     *
new_File(char *name)
{
   char      buf[256], *s, *e, *suf;

   NEWVAR(File, ret);
   e = strrchr(name, '/');
   if (e)
      ret->name_of_File = strdup(e + 1);
   else
      ret->name_of_File = strdup(name);
   ret->origname_of_File = strdup(name);
   if (name[0] == '/')
      strncpy(buf, name, sizeof(buf) - 3);
   else
      snprintf(buf, sizeof(buf) - 3, "%s/%s", outdir, name);
   s = strrchr(buf, '.');
   e = strrchr(buf, '/');
   if (pc_flag)
    {
       if (asm_flag)
	  suf = ".s";
       else
	  suf = ".c";
    }
   else if (pcode_flag)
      suf = ".po";
   else
      suf = ".c";

   if (s && (!e || e < s))
      strcpy(s, suf);
   else
      strcpy(buf + strlen(buf), suf);
   ret->cname_of_File = strdup(buf);
   s = strrchr(buf, '/');
   if (s)
      s++;
   else
      s = buf;
   ret->s_cname_of_File = strdup(s);

   if (pcode_flag && main_flag)
    {
       free(name);
       ret->name_of_File = strdup("MAIN.prg");
    }

   init_Coll(&ret->Coll_functions_of_File, NULL, compare_Function);
   init_Coll(&ret->Coll_unsortedFunctions_of_File, NULL, NULL);
   init_Coll(&ret->Coll_externFunctions_of_File, free, strcasecmp);
   init_Coll(&ret->Coll_externModules_of_File, free, strcasecmp);
   init_Coll(&ret->Coll_undeclExternFunctions_of_File, free, strcasecmp);

   init_Coll(&ret->Coll_numbers_of_File, NULL, compareConstNode);
   init_Coll(&ret->Coll_unsortedNumbers_of_File, NULL, NULL);
   init_Coll(&ret->Coll_strings_of_File, NULL, compareConstNode);
   init_Coll(&ret->Coll_unsortedStrings_of_File, NULL, NULL);

   init_Coll(&ret->Coll_codeblocks_of_File, NULL, NULL);
   init_Coll(&ret->Coll_names_of_File, free, strcmp);

   ret->Node_ctext_of_File = new_OperListNode();
   ret->Node_ctextStatic_of_File = new_OperListNode();

   return ret;
}
