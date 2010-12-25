static void
process_args(int argc, char **argv, int *Argcp, char ***Argvp)
{
   int i, j;

   for (i = 0; i < argc; i++)
   {
      int found = 0;

      for (j = 0; j < HIDDEN_NUM; j++)
      {
	 struct OptionAction *op;

	 int l, l1;

	 char *ap;

	 op = hidden_options + j;
	 ap = argv[i];

	 l = strlen(op->name);
	 l1 = strlen(ap);
	 if ((l <= l1) && (l == l1 || ap[l] == '=') && !memcmp(op->name, ap, l))
	 {
	    op->action(ap);
	    found = 1;
	    break;
	 }
      }
      if (!found)
      {
	 int l;

	 l = (*Argcp)++;
	 (*Argvp) = (char **) realloc(*Argvp, sizeof(char *) * (l + 1));

	 (*Argvp)[l] = argv[i];
      }
   }
}
