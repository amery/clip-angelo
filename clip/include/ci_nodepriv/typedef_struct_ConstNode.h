typedef struct ConstNode
{
   Node      node;
   int       type;
   int       no;
   char     *val;
   char     *rval;
   Function *func;
   int       len;
}
ConstNode;
