typedef struct Node
{
   ListEl listel;
   List list;
   long line;
   int file;
   int deep;
   int seqNo;
   int isExec:1, isExpr:1, isConst:1, isLval:1, isInit:1, isExit:1, isLoop:1,
      isNum:1, isStr:1, isLog:1, isArray:1, isTop:1, isAssign:1, isMinus:1,
      isField:1, isMacro:1, isExprList:1, isCode:1, isArrEl:1, isFMemvar:1, isRef:1, isNil:1, isLocal:1, isMemvar:1, isAssignLval:1;
   int (*pass) (void *self, Pass passno, int level, void *par);	/* traverser */
   const char *name;
}
Node;
