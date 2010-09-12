typedef struct VarNode
{
   Node node;
   Var *vp;
   int lval;
   int isArg;
   int p1;
   Function *fp;
}
VarNode;
