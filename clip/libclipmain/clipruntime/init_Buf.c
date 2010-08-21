
void
init_Buf(OutBuf * bp)
{
   bp->buf_of_OutBuf = bp->ptr_of_OutBuf = (char *) malloc(64);
   bp->end_of_OutBuf = bp->buf_of_OutBuf + 64;
}
