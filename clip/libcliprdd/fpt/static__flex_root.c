static FLEX_TREE *
_flex_root(FLEX_TREE * tree)
{
   FLEX_TREE *root = tree;

   while (root->parent)
      root = root->parent;
   return root;
}
