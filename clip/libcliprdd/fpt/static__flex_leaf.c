static FLEX_TREE *
_flex_leaf(FLEX_TREE * tree)
{
   FLEX_TREE *leaf = tree;

   while (leaf->child)
      leaf = leaf->child;
   return leaf;
}
