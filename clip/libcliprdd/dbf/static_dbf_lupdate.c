static int
dbf_lupdate(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   DBF_HEADER hdr;

   int er;

   if ((er = rdd_read(ClipMachineMemory, &rd->file, 0, sizeof(DBF_HEADER), &hdr, __PROC__)))
      return er;
   _clip_retdc(ClipMachineMemory, 1900 + (hdr.yy > 80 ? hdr.yy : hdr.yy + 100), hdr.mm, hdr.dd);
   return 0;
}
