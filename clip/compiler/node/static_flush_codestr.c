static void
flush_codestr(CodestrNode * np, void *nod)
{
   if (np->buf->ptr_of_StrBuf > np->buf->buf_of_StrBuf)
   {
      Node *sp;

      putByte_StrBuf(np->buf, 0);
      sp = new_StringConstNode(np->buf->buf_of_StrBuf);
      np->buf->buf_of_StrBuf = np->buf->ptr_of_StrBuf = np->buf->end_of_StrBuf = 0;
      np->buf->size_of_StrBuf = 0;

      append_Coll(&np->coll, sp);
   }
   if (nod)
      append_Coll(&np->coll, nod);
}
