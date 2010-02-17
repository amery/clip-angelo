/*
 	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

 */

#ifndef CI_NODEPRIV_H
#define CI_NODEPRIV_H

typedef struct
{
   Node node;
   Node *expr;
}
OperExprNode;

#define CONST_NUMBER 0
#define CONST_STRING 1
#define CONST_NIL 2
#define CONST_LOGIC 3
#define CONST_ISTRING 4

typedef struct
{
   Node node;
   int type;
   int no;
   char *val;
   char *rval;
   Function *func;
   int len;
}
ConstNode;

typedef struct
{
   Node node;
   VarColl *cp;
   char *name;
}
DefNode;

typedef struct
{
   Node node;
   Var *vp;
   int lval;
   int isArg;
   int p1;
   Function *fp;
}
VarNode;

typedef struct
{
   Node node;
   Var *vp;
   long space;
   char *spacename;
}
CreateVarNode;

typedef struct
{
   Node node;
   Node *expr;
   int i;
}
ExprNode;

typedef struct
{
   Node node;
   char *name;
   long hash;
   int argc;
   int isC;
   Function *func;
   Function *cfunc;
   int rest;
   int byref;
}
CallNode;

typedef struct
{
   Node node;
   Node *var;
   Node *expr;
   int op;
   int field_flag;
}
AssignNode;

typedef struct
{
   Node node;
   char *area;
   Node *areaExpr;
   char *name;
   Node *nameExpr;
   Node *expr;
}
AssignFieldNode;

typedef struct
{
   Node node;
   Coll *cp;
}
NewArrayNode;

typedef struct
{
   Node node;
   char *name;
   char *area;
   Node *nameExpr;
   Node *areaExpr;
}
FieldNode;

typedef struct
{
   Node node;
   char *name;
   long hash;
   int argc;
   Node *obj;
   int rest;
}
ObjNode;

typedef struct
{
   Node node;
   Function *func;
   Function *fp;
   Coll inits;
}
CodeNode;

typedef struct
{
   Node node;
   char *ctext;
}
LangNode;

typedef struct
{
   Node node;
   Node *arr;
   int dim;
   Node *expr;
   int op;
   Node *oarr;
}
ArrElNode;

typedef struct
{
   Node node;
   Node *name;
   int argc;
}
CallNameNode;

typedef struct
{
   Node node;
   Node *expr1;
   Node *expr2;
   int op;
}
OpNode;

typedef struct
{
   Node node;
   Node *expr;
   int isIncr;
   int isPost;
}
IncrNode;

typedef struct
{
   Node node;
   Coll *elseifs;
   Node *elselist;
   int labelNo;
}
IfNode;

typedef struct
{
   Node node;
   Node *expr;
   Node *list;
   int labelNo;
}
WhileNode;

typedef struct
{
   Node node;
   int isLoop;
   int offs;
   int processed;
}
LoopExitNode;

typedef struct
{
   Node node;
   Node *var;
   Node *init;
   Node *to;
   Node *step;
   Node *list;
   int labelNo;
   int keys;
}
ForNode;

typedef struct
{
   Node node;
   Node *list;
   Node *using;
   Node *recover;
   int seqNo;
   int curSeqNo;
   int toffs;
   int seqLevel;
}
SeqNode;

typedef struct
{
   Node node;
   Node *expr;
   char *name;
   Function *fp;
   int selfref;
}
RefNode;

typedef struct
{
   Node node;
   Node *expr;
   StrBuf *buf;
   Coll coll;
   VarColl *params;
   int block;
}
CodestrNode;

typedef struct
{
   Node node;
   Node *expr;
   Coll *cases;
   Node *other;
   Coll *labels;
}
SwitchNode;

typedef struct
{
   Node node;
   int last;
}
ExprListNode;

typedef struct
{
   Node node;
   Coll coll;
   int last;
}
ExprArrNode;

#endif
