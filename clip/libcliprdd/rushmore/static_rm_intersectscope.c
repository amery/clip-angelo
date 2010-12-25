static int
rm_intersectscope(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_FILTER * fp,
		  unsigned int *map, int oper, char *lval, char *rval, int bytes, int *optimize, int test, int *ic, const char *__PROC__)
{
   int i;

   ClipVar v, vv /*,vt,vvt */ ;

   ClipVar *vp /*,*vpt */ ;

   char *b = NULL;

   char *e = NULL;

   char *l = malloc(strlen(lval) + 1);

   char *r = malloc(strlen(rval) + 1);

   char *s, *d, *l1, *r1;

   if (ic)
      *ic = 0;
   memset(&v, 0, sizeof(ClipVar));
   memset(&vv, 0, sizeof(ClipVar));
/*
	memset(&vt,0,sizeof(ClipVar));
	memset(&vvt,0,sizeof(ClipVar));
*/
   *optimize = 0;
   for (s = lval, d = l; *s; s++)
   {
      if (*s != ' ')
	 *(d++) = *s;
   }
   *d = 0;
   while ((d = strstr(l, "FIELD->")))
      memmove(d, d + 7, strlen(d + 7) + 1);
   for (s = rval, d = r; *s; s++)
   {
      if (*s != ' ')
	 *(d++) = *s;
   }
   *d = 0;
   while ((d = strstr(r, "FIELD->")))
      memmove(d, d + 7, strlen(d + 7) + 1);
   l1 = strdup(l);
   l1[strlen(l1) - 1] = 0;
   r1 = strdup(r);
   r1[strlen(r1) - 1] = 0;
   for (i = 0; i < rd->ords_opened; i++)
   {
      if (rd->orders[i]->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	 continue;
      if (rd->orders[i]->unique)
	 continue;
      if ((strcasecmp(rd->orders[i]->expr, l) == 0) || (strcasecmp(rd->orders[i]->expr, l1 + 1) == 0))
      {
	 b = rval;
	 e = lval;
      }
      else if ((strcasecmp(rd->orders[i]->expr, r) == 0) || (strcasecmp(rd->orders[i]->expr, r1 + 1) == 0))
      {
	 b = lval;
	 e = rval;
	 oper = rm_invoper(oper);
      }
      if (b)
      {
	 char expr[1024];

	 if (!rd->orders[i]->vtbl->setscope)
	 {
	    *optimize = 0;
	    break;
	 }

	 rdd_expandmacro(rd->area, rd->rdhandle, b, expr);
	 if (_clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &vv))
	    break;
	 if (_clip_eval(ClipMachineMemory, _clip_vptr(&vv), 0, NULL, &v))
	    break;
	 vp = _clip_vptr(&v);
/*
			rdd_expandmacro(rd->area,rd->rdhandle,e,expr);
			if(_clip_eval_macro(ClipMachineMemory,expr,strlen(expr),&vvt))
				break;
			if(_clip_eval(ClipMachineMemory,_clip_vptr(&vvt),0,NULL,&vt))
				break;
			vpt = _clip_vptr(&vvt);
*/
	 if (ic)
	    *ic = rd->orders[i]->ic;
	 if (!test)
	 {
	    if (rm_checkscope(ClipMachineMemory, rd, map, fp->size, oper, e, vp, *ic, __PROC__))
	       goto err;
/*
				if(oper == RM_EQU){
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],vp,vp,map,
						fp->size,ClipMachineMemory->flags & EXACT_FLAG,__PROC__)) goto err;
				} else if(oper == RM_EEQU){
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],vp,vp,map,
						fp->size,1,__PROC__)) goto err;
				} else if(oper == RM_NEQU){
					unsigned int* tm = calloc(sizeof(unsigned int),bytes);
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],vp,vp,tm,
						fp->size,0,__PROC__)) goto err;
					rm_not(map,tm,bytes);
				} else if(oper == RM_LAR){
					unsigned int* tm = calloc(sizeof(unsigned int),bytes);
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],vp,NULL,map,
						fp->size,0,__PROC__)) goto err;
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],vp,vp,tm,
						fp->size,0,__PROC__)) goto err;
					for(j=0;j<bytes;j++)
						map[j] ^= tm[j];
					free(tm);
				} else if(oper == RM_LARE){
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],vp,NULL,map,
						fp->size,0,__PROC__)) goto err;
				} else if(oper == RM_LES){
					unsigned int* tm = calloc(sizeof(unsigned int),bytes);
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],NULL,vp,map,
						fp->size,0,__PROC__)) goto err;
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],vp,vp,tm,
						fp->size,0,__PROC__)) goto err;
					for(j=0;j<bytes;j++)
						map[j] ^= tm[j];
					free(tm);
				} else if(oper == RM_LESE){
					if(rd->orders[i]->vtbl->setscope(ClipMachineMemory,rd,rd->orders[i],NULL,vp,map,
						fp->size,0,__PROC__)) goto err;
				}
*/
	 }
	 *optimize = 2;
	 break;
      }
   }
   if (i == rd->ords_opened)
      ClipMachineMemory->m6_error = 2009;
   _clip_destroy(ClipMachineMemory, &v);
   _clip_destroy(ClipMachineMemory, &vv);
/*
	_clip_destroy(ClipMachineMemory,&vt);
	_clip_destroy(ClipMachineMemory,&vvt);
*/
   free(l);
   free(r);
   free(l1);
   free(r1);
   if (!(*optimize))
      memset(map, 0xff, bytes << 2);
   return 0;
 err:
   _clip_destroy(ClipMachineMemory, &v);
   _clip_destroy(ClipMachineMemory, &vv);
   free(l);
   free(r);
   free(l1);
   free(r1);
   return 1;
}
