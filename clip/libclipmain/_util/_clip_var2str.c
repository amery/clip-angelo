void
_clip_var2str(ClipMachine * ClipMachineMemory, ClipVar * vp, char **strp, long *lenp, int method)
{
   OutBuf buf;

   Coll refs;

   init_Buf(&buf);
   init_Coll(&refs, 0, refcmp);

   put_var(ClipMachineMemory, vp, &buf, &refs);
   switch (method)
   {
   case 1:			/* uuencode */
      _clip_uuencode(buf.buf_of_OutBuf, buf.ptr_of_OutBuf - buf.buf_of_OutBuf, strp, lenp, 1);
      break;
   case 2:			/* compress */
   default:			/* no either uuencode nor compress */
      *strp = buf.buf_of_OutBuf;
      *lenp = buf.ptr_of_OutBuf - buf.buf_of_OutBuf;
      break;
   }

   if (method == 1 /*|| method == 2 */ )
      destroy_Buf(&buf);

   destroy_Coll(&refs);
}
