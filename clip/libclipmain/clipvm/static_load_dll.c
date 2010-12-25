static int
load_dll(ClipMachine * ClipMachineMemory, const char *name, struct Coll *names, ClipVar * resp)
{
   void *hp;

   char buf[256], *s, *e;

   char uname[128];

   const char **spp;

   ClipModule *entry;

   ClipFunction **fpp;

   ClipNameEntry *np;

   ClipFile **cpp;

   struct DBFuncTable **dpp;

   int l, ret = 0, i;

   s = strrchr(name, '/');
   if (s)
      snprintf(buf, sizeof(buf), "%s", name);
   else
      snprintf(buf, sizeof(buf), "%s/lib/%s ", CLIPROOT, name);

   if (!loaded_dlls)
   {
      loaded_dlls = new_Coll(free, strcmp);
   }
   else
   {
      if (search_Coll(loaded_dlls, buf, 0))
	 return 0;
   }

#ifdef OS_MINGW
   hp = lt_dlopen(buf);
#else
   hp = dlopen(buf, RTLD_NOW);
#endif
   if (!hp)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "shared loading problem: %s: file %s", dlerror(), buf);
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   insert_Coll(loaded_dlls, strdup(buf));

/*
   path/name.ext -> name_module entry symbol
 */

   s = strrchr(name, '/');
   if (!s)
      s = (char *) name;
   else
      s = s + 1;
   e = strchr(s, '.');
   if (e)
      l = e - s;
   else
      l = strlen(s);
   if (l > sizeof(uname))
      l = sizeof(uname);

   for (i = 0; i < l; i++, s++)
   {
      if (*s == '-')
	 uname[i] = '_';
      else
	 uname[i] = toupper(*s);
   }
   uname[l] = 0;
   snprintf(buf, sizeof(buf), "clip__MODULE_%s", uname);

#ifdef OS_MINGW
   entry = (ClipModule *) lt_dlsym(hp, buf);
#else
   entry = (ClipModule *) dlsym(hp, buf);
#endif

   if (!entry)
   {
      _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "shared '%s' fetch name '%s' problem: %s", name, buf, dlerror());
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   for (np = entry->ClipNameEntry_cfunctions_of_ClipModule; np && np->ClipFunction_function_of_ClipNameEntry; ++np)
      _clip_register_hash(ClipMachineMemory, np->ClipFunction_function_of_ClipNameEntry, np->hash_of_ClipNameEntry);

   for (fpp = entry->ClipFunction_inits_of_ClipModule; fpp && *fpp; ++fpp)
      _clip_main_func(ClipMachineMemory, *fpp, _clip_argc, _clip_argv, _clip_envp);

   for (fpp = entry->ClipFunction_exits_of_ClipModule; fpp && *fpp; ++fpp)
   {
      ClipMachineMemory->cexits = (ClipFunction **) realloc(ClipMachineMemory->cexits, (ClipMachineMemory->ncexits + 1) * sizeof(ClipFunction *));
      ClipMachineMemory->cexits[ClipMachineMemory->ncexits] = *fpp;
      ++ClipMachineMemory->ncexits;
   }

   for (spp = entry->pfunctions_of_ClipModule; spp && *spp; ++spp)
      if (_clip_load(ClipMachineMemory, *spp, 0, 0))
	 ++ret;
/*
	if (entry->cpfile && _clip_register_file(ClipMachineMemory, entry->cpfile))
		++ret;
*/
   for (cpp = entry->cpfiles_of_ClipModule; cpp && *cpp; ++cpp)
      if (_clip_register_file(ClipMachineMemory, *cpp))
	 ++ret;

   for (dpp = entry->dbdrivers_of_ClipModule; dpp && *dpp; ++dpp)
      if (_clip_register_driver(ClipMachineMemory, *dpp))
	 ++ret;

   add_ClipVect(&ClipMachineMemory->dlls, hp);

   return ret;
}
