static int
__cdx_addkey(ClipMachine * ClipMachineMemory, int area, RDD_ORDER * ro, ClipVar * v, DbfLocale * loc, unsigned int recno, const char *__PROC__)
{
   CDX_HEADER hdr;

   int found;

   int len = ro->bufsize;

   int er;

   char *newkey;

   ro->level = 0;
   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
   {
      ClipVar vv, *vp;

      if ((er = rdd_calc(ClipMachineMemory, area, &ro->bforexpr, &vv, 0)))
	 return er;
      vp = _clip_vptr(&vv);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	 return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
      if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
      {
	 _clip_destroy(ClipMachineMemory, &vv);
	 return 0;
      }
      _clip_destroy(ClipMachineMemory, &vv);
   }
   newkey = malloc(ro->keysize);
   if (v)
   {
      if ((er = cdx_formatkey(ClipMachineMemory, ro, v, newkey, __PROC__)))
	 return er;
      if (v->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      {
	 len = min(v->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, ro->bufsize - (ro->type == 'X' ? 2 : 0));
      }
      else if (ro->type != 'L')
      {
	 char *s = newkey + ro->bufsize - 1;

	 while ((s >= newkey) && (*s == 0))
	    s--;
	 len = s - (char *) newkey + 1;
      }
   }
   else
   {
      ClipVar vv, *vp;

      if ((er = rdd_calc(ClipMachineMemory, area, &ro->block, &vv, 0)))
	 goto err1;
      vp = _clip_vptr(&vv);
      if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, newkey, __PROC__)))
	 goto err1;
      _clip_destroy(ClipMachineMemory, &vv);
   }

   _CDX_HEADER(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);

   /* Note: _cdx_back_search_tree()'s up ro->levels of ro->stack may be incorrect */
   if (ro->unique)
   {
      if ((er = _cdx_search_tree(ClipMachineMemory, ro, newkey, ro->bufsize, 0, &found, NULL, __PROC__)))
	 goto err1;
      if (found)
	 goto cont;
   }
   ro->level = 0;
   if ((er = _cdx_search_tree(ClipMachineMemory, ro, newkey, ro->bufsize, recno, &found, NULL, __PROC__)))
      goto err1;
   if (found)
      goto cont;
   if (!v)
   {
      if (ro->type == 'C')
      {
	 char *s = newkey + ro->bufsize - 1;

	 if (ro->binary)
	    while (*s == 0)
	       (s--, len--);
	 else
	    while (*s == ' ')
	       (s--, len--);
	 if (s != newkey + ro->bufsize - 1)
	    *(s + 1) = 0;
      }
      else if (ro->type != 'L')
      {
	 char *s = newkey + ro->bufsize - 1;

	 while ((s >= newkey) && (*s == 0))
	    s--;
	 len = s - (char *) newkey + 1;
      }
   }
   if ((ro->type == 'C' || (ro->type == 'X' && newkey[0] == type_weight(CHARACTER_type_of_ClipVarType))) && !ro->binary)
      loc_write(loc, (unsigned char *) newkey, len);
   er = _cdx_addkey(ClipMachineMemory, area, loc, v, ro, newkey, len, recno, __PROC__);
 err1:
   free(newkey);
   return er;
 cont:
   free(newkey);
   return 0;
}
