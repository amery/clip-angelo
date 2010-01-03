int
clip_DBRECORDINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRECORDINFO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       cmd = _clip_parni(ClipMachineMemory, 1);

   unsigned int rec = _clip_parni(ClipMachineMemory, 2);

   int       oldrecno, er;


   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);

   oldrecno = wa->rd->recno;

   if (rec)
      wa->rd->recno = rec;

   switch (cmd)
    {
    case DBRI_DELETED:
       {
	  int       d;

	  if ((er = rdd_deleted(ClipMachineMemory, wa->rd, &d, __PROC__)))
	     goto err;
	  _clip_retl(ClipMachineMemory, d);
	  break;
       }
    case DBRI_LOCKED:
       {
	  int       r = 0, i;

	  for (i = 0; i < wa->rd->nlocks; i++)
	   {
	      if (wa->rd->locks[i] == wa->rd->recno)
	       {
		  r = 1;
		  break;;
	       }
	   }
	  _clip_retl(ClipMachineMemory, r);
	  break;
       }
    case DBRI_RECSIZE:
       {
	  _clip_retni(ClipMachineMemory, wa->rd->recsize);
	  break;
       }
    case DBRI_RECNO:
       {
	  _clip_retni(ClipMachineMemory, wa->rd->recno);
	  break;
       }
    case DBRI_UPDATED:
       {
	  _clip_retl(ClipMachineMemory, 0);
	  break;
       }
    }
   wa->rd->recno = oldrecno;
   return 0;
 err:
   return er;
}
