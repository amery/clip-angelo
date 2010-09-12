int
clip_DBUSEAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBUSEAREA";

   int newArea = _clip_parl(ClipMachineMemory, 1);

   const char *driver = _clip_parc(ClipMachineMemory, 2);

   const char *name = _clip_parc(ClipMachineMemory, 3);

   const char *alias = _clip_parc(ClipMachineMemory, 4);

   char *alname;

   int shared = _clip_parl(ClipMachineMemory, 5);

   int readonly = _clip_parl(ClipMachineMemory, 6);

   int tempo = _clip_parl(ClipMachineMemory, 7);

   char data_driver[4];

   char idx_driver[4];

   char memo_driver[4];

   RDD_DATA *rd;

   DBWorkArea *wa = NULL;

   int no, i, re, er;

   const char deftrig[] = "SX_DEFTRIGGER";

   int *err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   ClipMachineMemory->neterr = 0;

   CHECKOPT1(1, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(5, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(6, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(7, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 5) == UNDEF_type_of_ClipVarType)
      shared = !(ClipMachineMemory->flags & EXCLUSIVE_FLAG);

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_db_driver;
   if ((er = db_driver(ClipMachineMemory, driver, data_driver, idx_driver, memo_driver, __PROC__)))
      goto err;

   if (ClipMachineMemory->areas->count_of_ClipVect == 0)
      newArea = 1;
   if (!newArea)
   {
      no = ClipMachineMemory->curArea;
      wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[no];
      if ((er = close_area(ClipMachineMemory, wa, __PROC__)))
	 return er;
   }
   else
   {
      for (no = 0; no < ClipMachineMemory->areas->count_of_ClipVect; no++)
	 if (ClipMachineMemory->areas->items_of_ClipVect[no] == NULL)
	    break;
   }

   ClipMachineMemory->curArea = no;

   if (no == ClipMachineMemory->areas->count_of_ClipVect)
      add_ClipVect(ClipMachineMemory->areas, NULL);

   ClipMachineMemory->noerrblock++;
   errno = 0;
   er = rdd_usearea(ClipMachineMemory, data_driver, name, shared, readonly, tempo, &rd, __PROC__);
   ClipMachineMemory->noerrblock--;
   if (er)
   {
      if (*err == 32)
      {
	 ClipMachineMemory->neterr = 1;
	 return 0;
      }
      else
      {
//                      rdd_err(ClipMachineMemory,EG_OPEN,errno,__FILE__,__LINE__,__PROC__,er_open);
	 goto err;
      }
   }

   wa = (DBWorkArea *) calloc(1, sizeof(DBWorkArea));
   strcpy(wa->driver, driver);
   strcpy(wa->idx_driver, idx_driver);
   strcpy(wa->memo_driver, memo_driver);
   wa->rd = rd;

   wa->name = malloc(strlen(name) + 1);
   strcpy(wa->name, name);

   if (alias)
      alname = (char *) alias;
   else
      alname = wa->rd->name;
   {
      /* Uri: 2006-12-29 Strip long alias names to 10 chars */
      char *s;

      int len = strlen(alname);

      len = len > 10 ? 10 : len;
      wa->alias = malloc(len + 1);
      strncpy(wa->alias, alname, len);
      wa->alias[len] = 0;
      s = wa->alias;
      while ((s = strchr(s, '.')))
	 *s = '_';
   }

   wa->rd->area = no;

   _clip_upstr(wa->alias, strlen(wa->alias));

   for (i = 0; i < wa->rd->nfields; i++)
   {
      if (wa->rd->fields[i].type == 'M' || (wa->rd->fields[i].type == 'V' && wa->rd->fields[i].len >= 6) || (wa->rd->fields[i].type == 'P') || (wa->rd->fields[i].type == 'G'))
      {
	 char *s;

	 char *tmpname = strdup(name);

	 if ((s = strrchr(tmpname, '.')))
	 {
	    if (!strrchr(s, '/') && !strrchr(s, '\\'))
	       *s = 0;
	 }
	 if ((er = rdd_setmemo(ClipMachineMemory, wa->rd, wa->memo_driver, tmpname, __PROC__)))
	 {
	    free(tmpname);
	    goto err;
	 }
	 free(tmpname);
	 break;
      }
   }

   wa->used = 1;
   wa->found = 0;
   wa->rd_item = _clip_store_c_item(ClipMachineMemory, (void *) wa->rd, _C_ITEM_TYPE_RDD, destroy_rdd_data);

   wa->no = no;
   ClipMachineMemory->areas->items_of_ClipVect[no] = wa;

   wa->aliasHash = _clip_casehashword(wa->alias, strlen(wa->alias));
   if (!HashTable_insert(ClipMachineMemory->aliases, wa, wa->aliasHash))
   {				/* ||
				   (strlen(wa->alias)==1 && wa->alias[0]>='A' && wa->alias[0]<='L')){ */
      char ermsg[100];

      _clip_destroy_c_item(ClipMachineMemory, wa->rd_item, _C_ITEM_TYPE_RDD);
      HashTable_remove(ClipMachineMemory->aliases, wa->aliasHash);
      ClipMachineMemory->areas->items_of_ClipVect[wa->no] = NULL;
      snprintf(ermsg, sizeof(ermsg), _clip_gettext("Alias already in use: %s"), wa->alias);
      er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, ermsg);
      goto err;
   }

   if (ClipMachineMemory->flags1 & AUTOPEN_FLAG)
   {
      char iname[1024];

      char *s;

      int i;

      RDD_INDEX_VTBL *ivtbl = rdd_indexdriver(ClipMachineMemory, wa->idx_driver, __PROC__);

      if (ivtbl && ivtbl->ismulti)
      {
	 strcpy(iname, wa->rd->path);
	 if ((s = strrchr(iname, '.')))
	    if (s[1] != '/' && s[1] != '\\')
	       *s = 0;
	 for (i = 0; i < *ClipMachineMemory->nidx_drivers; i++)
	    if (strcasecmp(wa->idx_driver, (*ClipMachineMemory->idx_drivers)[i]->id) == 0)
	    {
	       strcat(iname, (*ClipMachineMemory->idx_drivers)[i]->suff);
	       break;
	    }
	 if (access(iname, F_OK) == 0)
	 {
	    if ((er = rdd_setindex(ClipMachineMemory, wa->rd, NULL, wa->idx_driver, iname, NULL, 1, __PROC__)))
	       goto err;
	    wa->rd->indices[0]->structural = 1;
	    READLOCK;
	    if ((er = rdd_setorder(ClipMachineMemory, wa->rd, ClipMachineMemory->autorder, __PROC__)))
	       goto err;
	    if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
	       goto err_unlock;
	    UNLOCK;
	 }
      }
   }

   wa->trighash = _clip_hashstr(deftrig);
   wa->trigger = strdup(deftrig);
   wa->trig_active = 1;
   if ((er = rdd_event(ClipMachineMemory, EVENT_PREUSE, wa->rd->area, 0, NULL, &re, __PROC__)))
      goto err;
   if (re)
   {
      if ((er = rdd_event(ClipMachineMemory, EVENT_POSTUSE, wa->rd->area, 0, NULL, &re, __PROC__)))
	 goto err;
   }
   else
   {
      if ((er = rdd_closearea(ClipMachineMemory, wa->rd, __PROC__)))
	 goto err;
      wa->rd = NULL;
      _clip_destroy_c_item(ClipMachineMemory, wa->rd_item, _C_ITEM_TYPE_RDD);
      HashTable_remove(ClipMachineMemory->aliases, wa->aliasHash);
      ClipMachineMemory->areas->items_of_ClipVect[wa->no] = NULL;
      er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, "EVENT_PREUSE discarded");
      goto err;
   }
   _clip_retni(ClipMachineMemory, wa->rd_item);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   if (wa && wa->trigger)
      free(wa->trigger);
   if (wa && wa->name)
      free(wa->name);
   if (wa && wa->alias)
      free(wa->alias);
   if (wa)
      free(wa);
   return er;
}
