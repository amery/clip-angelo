static void
_flex_freetree(FLEX_TREE * tree)
{
   free(tree->items);
   if (tree->child)
      _flex_freetree(tree->child);
   free(tree);
}
