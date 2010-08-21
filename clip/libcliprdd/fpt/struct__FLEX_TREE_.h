struct _FLEX_TREE_
{
   int root;
   int leaf;
   unsigned int offs;
   int count;
   int pos;
   FLEX_TREE *parent;
   FLEX_TREE *child;
   FLEX_ITEM *items;
};
