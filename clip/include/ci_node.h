/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */

#ifndef CI_NODE_H
#define CI_NODE_H

#include <stdio.h>
#include "ci_list.h"

void fprintfOffs(FILE * file, int offs, const char *format, ...);

struct Node;

typedef struct Var
{
   char *name;
   int no;
   int line;
   int pos;
   int file;
   struct Function *func;
   struct Node *init;
   Coll *arr;
   char *alias;
   struct Node *macro;
   int isRef:1;
   int isFld:1;
   int isParam:1;
   int isCodeParam:1;
   int isLocalRef:1;
   int pno;
   int level;
   struct Var *refvar;
   char *type;
}
Var;

typedef struct VarColl
{
   Coll coll;
   Coll unsorted;
   int type;
}
VarColl;

Var *new_Var(char *name);
Var *newInit_Var(char *name, struct Node *expr);
Var *newArr_Var(char *name, Coll * arr);
Var *newArrInit_Var(char *name, struct Node *exprlist);
Var *mnew_Var(struct Node *macro);
Var *mnewInit_Var(struct Node *macro, struct Node *expr);
Var *mnewArr_Var(struct Node *macro, Coll * arr);
Var *mnewArrInit_Var(struct Node *macro, struct Node *exprlist);
void delete_Var(void *var);

VarColl *new_VarColl();
void delete_VarColl(VarColl *);
int search_VarColl(VarColl * coll, char *name, int *index);
void add_VarColl(VarColl * coll, Var * var);
void rm_VarColl(VarColl * coll, Var * var);
void insert_VarColl(VarColl * coll, Var * var);

VarColl *new_VarColl1(Var * vp);

typedef enum
{
   Print,
   Destroy,
   CalcDeep,
   CText,
   CTextInit,
   CTextLval,
   CTextRef,
   OText,
   OTextLval,
   OTextRef,
   OTextLoop,
   OTextTrap,
   SearchMacro,
   TestConst,
   Codegen1,
   Codegen2,
   PrintSrc,
   Traverse,
}
Pass;

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

typedef struct
{
   int (*func) (Node * np, void *par);
   void *par;
}
TraversePar;

void init_Node(void *node, int (*pass) (void *, Pass, int, void *), const char *name);
int pass_Node(void *self, Pass pass, int level, void *par);
int traverse_Node(Node * node, int (*func) (Node * np, void *par), void *par);
int getCount_Node(void *node);
Node *new_Node();
void delete_Node(Node * node);
void append_Node(Node * list, Node * node);
void join_Node(Node * list, Node * node);
void prepend_Node(Node * list, Node * node);

Node *new_OperListNode();
Node *new_OperExprNode(Node * node);

Node *new_NumberConstNode(char *val, char *rval);
Node *new_StringConstNode(char *val);
Node *new_LocaleStringNode(char *val);
Node *new_LogicConstNode(int val);
Node *new_NilConstNode();
Node *new_VardefNode(VarColl * cp);
int compareConstNode(void *n1, void *n2);

Node *new_LocalDefNode(VarColl * cp, int err, int localref);
Node *new_LocalDefNode1(VarColl * cp);
Node *new_StaticDefNode(VarColl * cp);
Node *new_MemvarDefNode(VarColl * cp);
Node *new_FieldDefNode(VarColl * cp, char *alias);
Node *new_PublicDefNode(VarColl * cp);
Node *new_PrivateDefNode(VarColl * cp);
Node *new_ParametersDefNode(VarColl * cp);
Node *new_CreateVarNode(Var * vp, long space, char *spacename);

Node *new_ParamNode(Var * vp);
Node *new_LocalNode(Var * vp);
Node *new_StaticNode(Var * vp);
Node *new_MemvarNode(Var * vp);
Node *new_FMemvarNode(Var * vp);
Node *new_MemvarFNode(Var * vp);
Node *new_PublicNode(Var * vp);
Node *new_PrivateNode(Var * vp);
Node *new_ParameterNode(Var * vp);
Node *new_FieldNode(Var * vp);
Node *new2_FieldNode(char *name1, Node * expr1, char *name2, Node * expr2);
Node *new_AssignFieldNode(char *area, Node * areaExpr, char *name, Node * nameExpr, Node * expr);

Node *new_ArgNode(Node * expr, int byRef);
Node *new_RefNode(Node * expr);
Node *new_CallNode(char *name, Coll * argv, int rest);
Node *new2_CallNode(const char *name, Node * expr1, Node * expr2);
Node *new3_CallNode(const char *name, Node * expr1, Node * expr2, Node * expr3);
Node *new_CallNameNode(Node * name, Coll * argv);
Node *newList_CallNode(char *name, Node * exprlist);
Node *new_ExprListNode();
Node *new_ExprArrNode();
Node *new_AssignNode(Node * var, Node * expr, int op);

Node *new_ArrayInitNode(Node * exprlist);
Node *new_ArrayInitNodeN(Node * exprlist, int n);
Node *new_NewArrayNode(Coll * cp);

Node *new_MethodNode(Node * obj, char *name, Coll * argv, int rest);
Node *new_GetNode(Node * obj, char *name);
Node *new_SetNode(Node * obj, char *name, Node * expr);

Node *new_CodeNode(VarColl * params);
Node *new_ReturnNode(Node * expr, int isExprList);

Node *new_LangNode(char *ctext);
Node *new_ArrElNode(Node * arr, Node * expr, int op);

Node *new_MacroNode(Node * expr, int isexpr);

Node *new_PcountNode();
Node *new_PshiftNode();
Node *new_ParnNode(Node * expr);
Node *new_PallNode();

Node *new_IncrNode(Node * expr, int isIncr, int isPost);

Node *new_OpNode(Node * expr1, Node * expr2, int op);
Node *new_AndNode(Node * expr1, Node * expr2);
Node *new_OrNode(Node * expr1, Node * expr2);
Node *new_NotNode(Node * expr);
Node *new_MinusNode(Node * expr);

Node *new_IfNode(Coll * elseifs, Node * elselist);
Node *new_WhileNode(Node * expr, Node * list);
Node *new_LoopExitNode(int isLoop);
Node *new_ForNode(Node * var, Node * from, Node * to, Node * step, Node * list);
Node *new_ForeachNode(Node * var, Node * expr, Node * list, int keys);

Node *new_SeqNode(Node * list, Node * using, Node * recover, int seqNo);
Node *new_BreakNode(Node * expr, int isOper);

Node *new_NamespaceDefNode(char *name, VarColl * cp);

Node *new_NewArrNode(Node * exprlist);

Node *new_HashConstNode(char *name);
Node *new_AssignFmemvarNode(Node * var, Node * expr);

Node *new_SwapNode(Node * expr1, Node * expr2);

Node *new_CodestrNode(Node * expr, int block, VarColl * params);
Node *new_QuotNode(Node * expr);

Node *new_SwitchNode(Node * expr, Coll * cases, Node * otherwise);

Node *new_CGetNode(Coll * argv);

#endif
