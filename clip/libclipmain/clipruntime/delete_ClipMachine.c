void
delete_ClipMachine(ClipMachine * ClipMachineMemory)
{
   FILE *out;

   int i;

   ClipFunction **np, ****npp;

   int r;

   if (ClipMachineMemory->deleted)
      return;
   ClipMachineMemory->deleted = 1;

   if (_clip_profiler)
   {
      fini_profiler(ClipMachineMemory);
   }

   if (ClipMachineMemory == first_mp)
      first_mp = ClipMachineMemory->next;
   else
   {
      ClipMachine *ClipMachineMemoryp;

      for (ClipMachineMemoryp = first_mp; ClipMachineMemoryp && ClipMachineMemoryp->next; ClipMachineMemoryp = ClipMachineMemoryp->next)
      {
	 if (ClipMachineMemoryp->next == ClipMachineMemory)
	 {
	    ClipMachineMemoryp->next = ClipMachineMemory->next;
	    break;
	 }
      }
   }

   /*if (machineCount == 1) */
   {
      _clip_close_all(ClipMachineMemory);
      clip_RDDCLOSEALL(ClipMachineMemory);
   }

   for (i = ClipMachineMemory->nexits - 1; i >= 0; --i)
   {
      _clip_main_code(ClipMachineMemory, ClipMachineMemory->exits + i, _clip_argc, _clip_argv, _clip_envp);
      destroy_Block(ClipMachineMemory, ClipMachineMemory->exits + i);
   }

   for (i = ClipMachineMemory->ncexits - 1; i >= 0; --i)
      _clip_main_func(ClipMachineMemory, ClipMachineMemory->cexits[i], _clip_argc, _clip_argv, _clip_envp);

   for (npp = init_struct->_libexits; *npp; ++npp)
   {
      for (np = **npp; *np; ++np)
	 _clip_main_func(ClipMachineMemory, *np, _clip_argc, _clip_argv, _clip_envp);
   }
   for (np = init_struct->_exits; *np; ++np)
      _clip_main_func(ClipMachineMemory, *np, _clip_argc, _clip_argv, _clip_envp);

   clear_VarTable(ClipMachineMemory, ClipMachineMemory->privates);
   /*clear_VarTable(ClipMachineMemory, ClipMachineMemory->publics); */

   for (i = 0; i < ClipMachineMemory->dlls.count_of_ClipVect; ++i)
#ifdef OS_MINGW
      lt_dlclose(ClipMachineMemory->dlls.items_of_ClipVect[i]);
#else
      dlclose(ClipMachineMemory->dlls.items_of_ClipVect[i]);
#endif

   out = (FILE *) ClipMachineMemory->out;
   if (out != stdout)
      fclose(out);
   out = (FILE *) ClipMachineMemory->extra;
   if (out)
      fclose(out);
   out = (FILE *) ClipMachineMemory->alt;
   if (out)
      fclose(out);
   _clip_close_printer(ClipMachineMemory);

   if (ClipMachineMemory->pbuf)
   {
      destroy_Buf(ClipMachineMemory->pbuf);
      free(ClipMachineMemory->pbuf);
   }

   delete_VarTable(ClipMachineMemory, ClipMachineMemory->privates);
   delete_spaces(ClipMachineMemory, ClipMachineMemory->spaces);
   delete_HashTable(ClipMachineMemory->aliases);
   delete_HashTable(ClipMachineMemory->fields);

   if (machineCount == 1)
   {
      delete_VarTable(ClipMachineMemory, ClipMachineMemory->publics);
      all_publics = 0;
      all_aliases = 0;
      all_fields = 0;
      for (r = HashTable_first(ClipMachineMemory->store); r; r = HashTable_next(ClipMachineMemory->store))
	 free(HashTable_current(ClipMachineMemory->store));
      delete_HashTable(ClipMachineMemory->store);
      all_store = 0;
   }

   if (machineCount == 1)
   {
      for (r = HashTable_first(ClipMachineMemory->hashnames); r; r = HashTable_next(ClipMachineMemory->hashnames))
	 free(HashTable_current(ClipMachineMemory->hashnames));
      delete_HashTable(ClipMachineMemory->hashnames);
      all_hashnames = 0;
   }

   _clip_delete(ClipMachineMemory, ClipMachineMemory->trapVar);
   _clip_delete(ClipMachineMemory, ClipMachineMemory->errorblock);
   _clear_trap(ClipMachineMemory);

/*	if (machineCount == 1)
	{*/
   destroy_ClipVect(ClipMachineMemory->areas);
   destroy_ClipVect(ClipMachineMemory->areaStack);
   free(ClipMachineMemory->areas);
   free(ClipMachineMemory->areaStack);
/*	}*/

   free(ClipMachineMemory->date_format);
   free(ClipMachineMemory->path);
   free(ClipMachineMemory->defaul);
   free(ClipMachineMemory->altfile);
   free(ClipMachineMemory->extrafile);
   if (ClipMachineMemory->rootpath != NULL)
      free(ClipMachineMemory->rootpath);
   free(ClipMachineMemory->delimchars);
   if (machineCount == 1)
   {
      delete_Functions(ClipMachineMemory, ClipMachineMemory->functions);
      all_functions = 0;
   }

   free(ClipMachineMemory->buf);

   for (i = 0; i < ClipMachineMemory->history.count_of_ClipVect; ++i)
      free(ClipMachineMemory->history.items_of_ClipVect[i]);

   destroy_ClipVect(&ClipMachineMemory->history);

   free(ClipMachineMemory->exits);
   free(ClipMachineMemory->cexits);
   /*free(*ClipMachineMemory->dbdrivers); */

   if (machineCount == 1)
      free(ClipMachineMemory->kbdbuf);

   destroy_ClipVect(&ClipMachineMemory->dlls);

   if (ClipMachineMemory->screen_base)
   {
      if (ClipMachineMemory->screen)
	 delete_Screen(ClipMachineMemory->screen);
      destroy_tty(ClipMachineMemory->screen_base);
      free(ClipMachineMemory->screen_base);
   }

   /*if ( machineCount == 1 ) */
   {
      /*all_windows = 0; */
      for (i = 0; i <= ClipMachineMemory->wnum; i++)
      {
	 ClipWindow *wp = ClipMachineMemory->windows + i;

	 if (wp->save_of_ClipWindow)
	    free(wp->save_of_ClipWindow);
      }
      free(ClipMachineMemory->windows);
   }

   --machineCount;

   free(ClipMachineMemory);
}
