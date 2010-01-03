static void
put_line(void *v, int level, StrBuf * out)
{
   VAR(Node, np, v);
   if (clic_line != np->line)
    {
       putByte_StrBuf(out, CLIP_LINE);
       putShort_StrBuf(out, np->line);
    }
   clic_line = np->line;
}
