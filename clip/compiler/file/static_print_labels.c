static void
print_labels(FILE * out, Coll * tree)
{
   int i, count;

	for (i = 0, count = tree->count_of_Coll; i < count; i++)
   {
		Label *cp = (Label *) tree->items_of_Coll[i];

      /*fprintf(out, "\t.p2align 4,,7\n"); */
		fprintf(out, ".L%d:\n", cp->label_of_Label);
		fprintf(out, "\tmovl $%s%s,%%eax\n", US, cp->str_of_Label);
      fprintf(out, "\tjmp .L2\n");
   }
}
