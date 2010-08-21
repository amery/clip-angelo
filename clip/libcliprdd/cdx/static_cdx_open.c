static int
cdx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__)
{
   RDD_ORDER *tagbag = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));

   RDD_ORDER *ro = NULL;

   char key[11];

   CDX_HEADER hdr;

   int f, i, stord, er;

   RDD_ORDER *h;

   char cntcdx[4];

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &hdr, __PROC__)))
      goto err;
   if (hdr.options != 0xe0)
   {
      free(tagbag);
      return idx_open(ClipMachineMemory, rd, ri, tag, __PROC__);
   }

   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, 0, tagbag, __PROC__)))
      goto err;

   tagbag->stack[0].page = _rdd_uint(hdr.root);
   tagbag->level = 0;

   ri->loc = rd->loc;
   if (tag)
   {
      int found;

      unsigned int newheader;

      char *e = key + 10;

      char cntcdx[4];

      memset(key, 0, sizeof(key));
      strncpy(key, tag, 10);
      while (!(*(--e)))
	 *e = ' ';
      _clip_upstr(key, 10);
      if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, key, 10, 0, &found, NULL, __PROC__)))
	 goto err;
      if (!found)
      {
	 er = rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, er_notag);
	 goto err;
      }
      if ((er = _cdx_recno(ClipMachineMemory, tagbag, tagbag->level, &newheader, __PROC__)))
	 goto err;
      ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
      if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, newheader, ro, __PROC__)))
	 goto err;

      rd->ords_opened++;
      rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
      rd->orders[rd->ords_opened - 1] = ro;
      ro->name = (char *) malloc(strlen(tag) + 1);
      strcpy(ro->name, tag);
      ro->orderno = rd->ords_opened - 1;
      ri->norders++;
      ri->orders = (RDD_ORDER **) realloc(ri->orders, ri->norders * sizeof(RDD_ORDER *));
      ri->orders[ri->norders - 1] = ro;
      ro->canadd = 1;
      if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	 goto err;
      ro->cntcdx = _rdd_backuint((unsigned char *) cntcdx);
   }
   else
   {
      int out;

      unsigned int header;

      char tagname[11];

      char *s;

      if ((er = _cdx_first(ClipMachineMemory, tagbag, &out, __PROC__)))
	 goto err;
      while (!out)
      {
	 if ((er = _cdx_recno_keyval(ClipMachineMemory, tagbag, tagbag->level, &header, tagname, __PROC__)))
	    goto err;
	 ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
	 if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, header, ro, __PROC__)))
	    goto err;

	 ro->canadd = 1;
	 rd->ords_opened++;
	 rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
	 rd->orders[rd->ords_opened - 1] = ro;
	 tagname[sizeof(tagname) - 1] = 0;
	 s = strchr(tagname, ' ');
	 if (s)
	    *s = 0;
	 ro->name = (char *) malloc(strlen(tagname) + 1);
	 strcpy(ro->name, tagname);
	 ro->orderno = rd->ords_opened - 1;
	 ri->norders++;
	 ri->orders = (RDD_ORDER **) realloc(ri->orders, ri->norders * sizeof(RDD_ORDER *));
	 ri->orders[ri->norders - 1] = ro;
	 if ((er = _cdx_next(ClipMachineMemory, rd, tagbag, &out, NULL, __PROC__)))
	    goto err;
	 if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
	    goto err;
	 ro->cntcdx = _rdd_backuint((unsigned char *) cntcdx);
      }
      stord = ri->orders[0]->orderno;
      do
      {
	 f = 1;
	 for (i = 0; i < ri->norders - 1; i++)
	 {
	    if (rd->orders[stord + i]->header > rd->orders[stord + i + 1]->header)
	    {
	       h = rd->orders[stord + i];
	       rd->orders[stord + i] = rd->orders[stord + i + 1];
	       rd->orders[stord + i + 1] = h;
	       rd->orders[stord + i]->orderno = stord + i;
	       rd->orders[stord + i + 1]->orderno = stord + i + 1;
	       f = 0;
	    }
	 }
      }
      while (!f);
      do
      {
	 f = 1;
	 for (i = 0; i < ri->norders - 1; i++)
	 {
	    if (ri->orders[i]->header > ri->orders[i + 1]->header)
	    {
	       h = ri->orders[i];
	       ri->orders[i] = ri->orders[i + 1];
	       ri->orders[i + 1] = h;
	       f = 0;
	    }
	 }
      }
      while (!f);
   }
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   if (ro)
      free(ro);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}
