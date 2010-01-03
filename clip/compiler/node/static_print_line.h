static void
print_line(void *v, int level, FILE * out)
{
   VAR(Node, np, v);
   if (clic_line != np->line)
    {
       fprintfOffs(out, level, "_frame.line_of_ClipFrame = %d;\n", np->line);
       if (genDebug)
	  fprintfOffs(out, level, "_clip_line(_mp);\n");
    }
   clic_line = np->line;
}
