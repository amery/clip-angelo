static void
print_labels(FILE * out, Coll * tree)
{
   int       i, count;

   for (i = 0, count = tree->count; i < count; i++)
    {
       Label    *cp = (Label *) tree->items[i];

      /*fprintf(out, "\t.p2align 4,,7\n"); */
       fprintf(out, ".L%d:\n", cp->label);
       fprintf(out, "\tmovl $%s%s,%%eax\n", US, cp->str);
       fprintf(out, "\tjmp .L2\n");
    }
}
