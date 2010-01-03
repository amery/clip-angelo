int
clip_M6_ORDCONDSET(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_ORDCONDSET";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   const char *cForCondition = _clip_parc(ClipMachineMemory, 1);

   ClipVar  *bForCondition = _clip_spar(ClipMachineMemory, 2);

   int       lAll = _clip_parl(ClipMachineMemory, 3);

   ClipVar  *bWhileCondition = _clip_spar(ClipMachineMemory, 4);

   ClipVar  *bEval = _clip_spar(ClipMachineMemory, 5);

   int       nInterval = _clip_parni(ClipMachineMemory, 6);

   int       nStart = _clip_parni(ClipMachineMemory, 7);

   int       nNext = _clip_parni(ClipMachineMemory, 8);

   int       nRecord = _clip_parni(ClipMachineMemory, 9);

   int       lRest = _clip_parl(ClipMachineMemory, 10);

   int       lDescend = _clip_parl(ClipMachineMemory, 11);

   int       lCurrent = _clip_parl(ClipMachineMemory, 14);

   int       lAdditive = _clip_parl(ClipMachineMemory, 17);

   int       lCustom = _clip_parl(ClipMachineMemory, 18);

   int       lNoOptimize = _clip_parl(ClipMachineMemory, 19);

   int       er;


   ClipMachineMemory->m6_error = 0;
   if (!wa)
      return 0;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT2(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT2(4, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT2(5, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);
   CHECKOPT1(6, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(7, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(8, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(9, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(10, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(11, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(14, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(17, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(18, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(19, LOGICAL_type_of_ClipVarType);

   if (wa->rd->os.cForCondition)
    {
       free(wa->rd->os.cForCondition);
       wa->rd->os.cForCondition = NULL;
    }
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bForCondition);
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bWhileCondition);
   _clip_destroy(ClipMachineMemory, &wa->rd->os.bEval);

   if (cForCondition)
    {
       wa->rd->os.cForCondition = malloc(strlen(cForCondition) + 1);
       strcpy(wa->rd->os.cForCondition, cForCondition);
    }

   if (bForCondition)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bForCondition, bForCondition);
   wa->rd->os.lAll = lAll;
   if (bWhileCondition)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bWhileCondition, bWhileCondition);
   if (bEval)
      _clip_clone(ClipMachineMemory, &wa->rd->os.bEval, bEval);
   wa->rd->os.nInterval = nInterval;
   wa->rd->os.nStart = (lAll ? 0 : nStart);
   wa->rd->os.nNext = nNext;
   wa->rd->os.nRecord = nRecord;
   wa->rd->os.lRest = lRest;
   wa->rd->os.lDescend = lDescend;
   wa->rd->os.lAdditive = lAdditive;
   wa->rd->os.lCurrent = lCurrent;
   wa->rd->os.lCustom = lCustom;
   wa->rd->os.lNoOptimize = lNoOptimize;

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
