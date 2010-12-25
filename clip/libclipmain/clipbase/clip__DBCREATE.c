int
clip__DBCREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBCREATE";

   const char *name = _clip_parc(ClipMachineMemory, 1);

   ClipVar *stru = _clip_par(ClipMachineMemory, 2);

   const char *driver = _clip_parc(ClipMachineMemory, 3);

   char data_driver[4];

   char idx_driver[4];

   char memo_driver[4];

   int i, er;

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, ARRAY_type_of_ClipVarType);
   CHECKOPT1(3, CHARACTER_type_of_ClipVarType);

   if ((er = db_driver(ClipMachineMemory, driver, data_driver, idx_driver, memo_driver, __PROC__)))
      return er;

   if ((er = rdd_create(ClipMachineMemory, data_driver, memo_driver, name, stru, __PROC__)))
      return er;
   for (i = 0; i < stru->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
   {
      ClipVar *ap, *lp;

      ap = _clip_vptr(stru->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);
      if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < 4)
	 continue;
      lp = _clip_vptr(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 2);
      ap = _clip_vptr(ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 1);
      if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
	 continue;

      /*if (toupper(stru->a.items[i].a.items[0].a.items[1].s.ClipBuf_str_of_ClipStrVar.buf[0]) == 'M') */
      if (strchr("MPG", toupper(ap->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0])) ||
	  (toupper(ap->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0]) == 'V' && lp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 6))
      {
	 char *mname = malloc(strlen(name) + 1);

	 char *s;

	 strcpy(mname, name);
	 if ((s = strrchr(mname, '.')) && strrchr(mname, '/') < s)
	    *s = 0;
	 if ((er = rdd_creatememo(ClipMachineMemory, memo_driver, mname, __PROC__)))
	 {
	    free(mname);
	    return er;
	 }
	 free(mname);
	 break;
      }
   }
   return 0;
 err:
   return er;
}
