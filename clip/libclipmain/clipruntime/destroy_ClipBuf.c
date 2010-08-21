void
destroy_ClipBuf(ClipBuf * buf)
{
   if (!buf)
      return;
   if (buf->buf_of_ClipBuf)
      free(buf->buf_of_ClipBuf);
}
