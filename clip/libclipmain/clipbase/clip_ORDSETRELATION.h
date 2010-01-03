int
clip_ORDSETRELATION(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDSETRELATION";

   int       type = _clip_parinfo(ClipMachineMemory, 1);

   ClipVar  *block = _clip_spar(ClipMachineMemory, 2);

   const char *expr = _clip_parc(ClipMachineMemory, 3);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   DBWorkArea *childwa = 0;

   int       er;


   CHECKWA(wa);
   CHECKARG2(1, NUMERIC_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   if (type == NUMERIC_type_of_ClipVarType)
    {
       childwa = get_area(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1), 0, 0);
    }
   else if (type == CHARACTER_type_of_ClipVarType)
    {
       int       i;

       DBWorkArea *twa;

       const char *alias = _clip_parc(ClipMachineMemory, 1);

       for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
	{
	   twa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
	   if (twa && twa->alias && !strcasecmp(alias, twa->alias))
	      break;
	}
       childwa = get_area(ClipMachineMemory, i + 1, 0, 0);
    }

   CHECKWA(childwa);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_setrelation(ClipMachineMemory, wa->rd, childwa->rd, block, expr, 1, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
