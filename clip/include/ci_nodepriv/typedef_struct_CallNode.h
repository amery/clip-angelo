typedef struct CallNode
{
   Node      node;
   char     *name;
   long      hash;
   int       argc;
   int       isC;
   Function *func;
   Function *cfunc;
   int       rest;
   int       byref;
}
CallNode;
