int
_rdd_fieldno(RDD_DATA * rd, long hash)
{
   RDD_FIELD *fp = (RDD_FIELD *) HashTable_fetch(rd->hashes, hash);

   int fno = fp - rd->fields;

   if ((fno >= rd->nfields) || (fno < 0))
      return -1;

   return fno;
}
