void
init_Node(void *node, int (*pass) (void *, Pass, int, void *), const char *name)
{
   VAR(Node, np, node);
   memset(np, 0, sizeof(Node));
   if (pass)
      np->pass = pass;
   else
      np->pass = pass_Node;
   init_List(&np->list);
   np->line = cl_line;
   np->file = currentFile();
   np->isExec = 1;
	np->seqNo = (curFunction ? curFunction->seqNo_of_Function : 0);
   np->name = name;
}
