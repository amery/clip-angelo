typedef struct IncrNode
{
   Node      node;
   Node     *expr;
   int       isIncr;
   int       isPost;
}
IncrNode;
