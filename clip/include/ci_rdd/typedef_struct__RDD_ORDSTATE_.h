typedef struct _RDD_ORDSTATE_
{
   char     *cForCondition;
   ClipVar   bForCondition;
   int       lAll;
   ClipVar   bWhileCondition;
   ClipVar   bEval;
   int       nInterval;
   int       nStart;
   int       nNext;
   int       nRecord;
   int       lRest;
   int       lDescend;
   int       lAdditive;
   int       lCurrent;
   int       lCustom;
   int       lNoOptimize;
   int       lIgnoreCase;
} RDD_ORDSTATE;
