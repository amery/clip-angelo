int
clip_DBORDERINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBORDERINFO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int cmd = _clip_parni(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   ClipVar *order = _clip_par(ClipMachineMemory, 3);

   RDD_ORDER *ro;

   int ord, er;

   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(3, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   ro = wa->rd->orders[ord];
   switch (cmd)
   {
   case DBOI_CONDITION:
   case DBOI_CUSTOM:
   case DBOI_HPLOCKING:
   case DBOI_ISCOND:
   case DBOI_ISDESC:
   case DBOI_KEYADD:
   case DBOI_KEYDELETE:
   case DBOI_KEYGOTO:
   case DBOI_LOCKOFFSET:
   case DBOI_ORDERCOUNT:
   case DBOI_SCOPEBOTTOM:
   case DBOI_SCOPEBOTTOMCLEAR:
   case DBOI_SCOPETOP:
   case DBOI_SCOPETOPCLEAR:
   case DBOI_SKIPUNIQUE:
      {
	 if ((er = wa->rd->orders[ord]->vtbl->info(ClipMachineMemory, wa->rd, ro, cmd, __PROC__)))
	    goto err;
	 break;
      }
   case DBOI_EXPRESSION:
      {
	 _clip_retc(ClipMachineMemory, ro->expr);
	 break;
      }
   case DBOI_FILEHANDLE:
      {
	 _clip_retni(ClipMachineMemory, -1);
	 break;
      }
   case DBOI_INDEXEXT:
      {
	 _clip_retc(ClipMachineMemory, ro->vtbl->suff);
	 break;
      }
   case DBOI_INDEXNAME:
      {
	 _clip_retc(ClipMachineMemory, ro->index->name);
	 break;
      }
   case DBOI_KEYCOUNT:
   case DBOI_KEYSINCLUDED:
      {
	 int c;

	 if ((er = ro->vtbl->lastkey(ClipMachineMemory, wa->rd, ro, &c, __PROC__)))
	    goto err;
	 _clip_retni(ClipMachineMemory, c);
	 break;
      }
   case DBOI_KEYDEC:
      {
	 _clip_retni(ClipMachineMemory, ro->dec);
	 break;
      }
   case DBOI_KEYSIZE:
      {
	 _clip_retni(ClipMachineMemory, ro->keysize);
	 break;
      }
   case DBOI_KEYTYPE:
      {
	 ClipVar vv;

	 char t[2];

	 if ((er = _clip_eval(ClipMachineMemory, &ro->block, 0, 0, &vv)))
	    goto err;
	 if (vv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	    t[0] = 'C';
	 else if (vv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	    t[0] = 'N';
	 else if (vv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
	    t[0] = 'D';
	 else if (vv.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	    t[0] = 'L';
	 else
	    t[0] = 'U';
	 t[1] = 0;
	 _clip_destroy(ClipMachineMemory, &vv);
	 _clip_retc(ClipMachineMemory, t);
	 break;
      }
   case DBOI_KEYVAL:
      {
	 if ((er = _clip_eval(ClipMachineMemory, &ro->block, 0, 0, RETPTR(ClipMachineMemory))))
	    goto err;
	 break;
      }
   case DBOI_NAME:
      {
	 _clip_retc(ClipMachineMemory, ro->name);
	 break;
      }
   case DBOI_NUMBER:
      {
	 _clip_retni(ClipMachineMemory, ord + 1);
	 break;
      }
   case DBOI_RECNO:
      {
	 int n;

	 if ((er = ro->vtbl->keyno(ClipMachineMemory, wa->rd, ro, &n, __PROC__)))
	    goto err;
	 _clip_retni(ClipMachineMemory, n);
	 break;
      }
   case DBOI_SETCODEBLOCK:
      {
	 _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), &ro->block);
	 break;
      }
   case DBOI_UNIQUE:
      {
	 _clip_retl(ClipMachineMemory, ro->unique);
	 break;
      }
   }
   return 0;
 err:
   return er;
}
