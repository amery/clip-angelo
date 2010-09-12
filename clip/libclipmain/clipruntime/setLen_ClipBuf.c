void
setLen_ClipBuf(ClipBuf * buf, int newlen)
{
   buf->buf_of_ClipBuf = (char *) realloc(buf->buf_of_ClipBuf, newlen);
   buf->len_of_ClipBuf = newlen;
}
