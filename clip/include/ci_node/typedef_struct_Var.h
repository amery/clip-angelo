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
