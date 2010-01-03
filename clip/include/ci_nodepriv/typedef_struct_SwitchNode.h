typedef struct SwitchNode
{
   Node      node;
   Node     *expr;
   Coll     *cases;
   Node     *other;
   Coll     *labels;
}
SwitchNode;
