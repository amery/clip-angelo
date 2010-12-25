static int
cdx_ii_opentag(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *tag, RDD_ORDER ** rop, const char *__PROC__)
{
   CDX_HEADER chdr;

   CDX_PAGE page;

   CDX_LEAF *leaf;

   CDX_BRANCH *branch;

   RDD_ORDER *tagbag = (RDD_ORDER *) calloc(1, sizeof(RDD_ORDER));

   RDD_ORDER *ro;

   char tn[11];

   unsigned int header;

   ClipVar v;

   int found, er;

   tagbag->level = 0;
   if ((er = _cdx_init_order(ClipMachineMemory, -1, ri, 0, tagbag, __PROC__)))
      goto err;
   _clip_var_str(tag, strlen(tag), &v);
   _clip_upstr(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

   _CDX_HEADER(tagbag, &chdr);
   tagbag->stack[0].page = _rdd_uint(chdr.root);
   strcpy(tn, tag);
   _clip_upstr(tn, strlen(tn));
   memset(tn + strlen(tn), 0x20, 10 - strlen(tn));
   tn[10] = 0;
   if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, tn, 10, 0, &found, NULL, __PROC__)))
      goto err;
   _CDX_PAGE(tagbag, tagbag->stack[tagbag->level].page, &page);
   if (!leaf)
      return rdd_err(ClipMachineMemory, EG_CORRUPTION, 0, __FILE__, __LINE__, __PROC__, er_badleaf);

   if ((er = _cdx_recno_keyval(ClipMachineMemory, tagbag, tagbag->level, &header, tn, __PROC__)))
      return er;

   if (!found)
      return rdd_err(ClipMachineMemory, EG_OPEN, 0, __FILE__, __LINE__, __PROC__, er_notag);

   ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
   if ((er = _cdx_init_order(ClipMachineMemory, -1, ri, header, ro, __PROC__)))
      return er;

   _clip_destroy(ClipMachineMemory, &v);

   ro->key = malloc(ro->bufsize);
   ro->iikey = malloc(ro->bufsize);
   ro->name = (char *) malloc(strlen(tag) + 1);
   strcpy(ro->name, tag);
   ri->norders++;
   ri->orders = (RDD_ORDER **) realloc(ri->orders, ri->norders * sizeof(RDD_ORDER *));
   ri->orders[ri->norders - 1] = ro;
   destroy_rdd_order(ClipMachineMemory, tagbag);
   *rop = ro;
   return 0;
 err:
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}
