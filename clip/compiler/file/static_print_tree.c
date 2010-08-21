static void
print_tree(FILE * out, Coll * tree, int beg, int end)
{
   int med;

   Label *lp, *rp, *cp;

   if (beg > end)
      return;

   med = (beg + end) / 2;

   fprintf(out, "#1234/%d/%d/%d\n", beg, med, end);

   if (beg == end)
   {
		cp = (Label *) tree->items_of_Coll[beg];

      /*fprintf(out, "\t.p2align 4,,7\n"); */
		fprintf(out, ".L%d:\n", cp->branch_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash_of_Label);
		fprintf(out, "\tje .L%d\n", cp->label_of_Label);
      fprintf(out, "\tjmp .L1\n");
      return;
   }
   else if ((beg + 1) == med && (end - 1) == med)
   {
		cp = (Label *) tree->items_of_Coll[med];
		lp = (Label *) tree->items_of_Coll[beg];
		rp = (Label *) tree->items_of_Coll[end];

      /*fprintf(out, "\t.p2align 4,,7\n"); */
		fprintf(out, ".L%d:\n", cp->branch_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", lp->hash_of_Label);
		fprintf(out, "\tje .L%d\n", lp->label_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash_of_Label);
		fprintf(out, "\tje .L%d\n", cp->label_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", rp->hash_of_Label);
		fprintf(out, "\tje .L%d\n", rp->label_of_Label);

      fprintf(out, "\tjmp .L1\n");
      return;
   }
   else if (beg == (end - 1))
   {
		cp = (Label *) tree->items_of_Coll[beg];
		rp = (Label *) tree->items_of_Coll[end];

      /*fprintf(out, "\t.p2align 4,,7\n"); */
		fprintf(out, ".L%d:\n", cp->branch_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash_of_Label);
		fprintf(out, "\tje .L%d\n", cp->label_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", rp->hash_of_Label);
		fprintf(out, "\tje .L%d\n", rp->label_of_Label);

      fprintf(out, "\tjmp .L1\n");
      return;
   }
   else
   {
		cp = (Label *) tree->items_of_Coll[med];
		lp = (Label *) tree->items_of_Coll[(beg + med - 1) / 2];
		rp = (Label *) tree->items_of_Coll[(med + 1 + end) / 2];

      /*fprintf(out, "\t.p2align 4,,7\n"); */
		fprintf(out, ".L%d:\n", cp->branch_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash_of_Label);
		fprintf(out, "\tje .L%d\n", cp->label_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash_of_Label);
		fprintf(out, "\tjl .L%d\n", lp->branch_of_Label);
		fprintf(out, "\tcmpl $%ld,%%eax\n", cp->hash_of_Label);
		fprintf(out, "\tjg .L%d\n", rp->branch_of_Label);

      fprintf(out, "\tjmp .L1\n");
   }

   print_tree(out, tree, beg, med - 1);
   print_tree(out, tree, med + 1, end);
}
