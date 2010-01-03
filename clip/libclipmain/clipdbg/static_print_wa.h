static void
print_wa(ClipMachine * ClipMachineMemory, DBWorkArea * wa)
{
   int       i;


   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
    {
       int       j;

       wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i];
       if (!wa || !wa->used)
	  continue;
       fprintf(_clip_dbg_out,
	       "no:%d name:%s alias:%s shared:%d ro:%d filter:%s\n",
	       wa->rd->area, wa->rd->name, wa->alias, wa->rd->shared,
	       wa->rd->readonly, wa->rd->filter && wa->rd->filter->sfilter ? wa->rd->filter->sfilter : "");
       fprintf(_clip_dbg_out, "\t%d fields:\n", wa->rd->nfields);
       for (j = 0; j < wa->rd->nfields; j++)
	{
	   RDD_FIELD *fp = wa->rd->fields + j;

	   fprintf(_clip_dbg_out, "\t\tno:%d\tname:%s\ttype:%c\tlen:%d\tdec:%d\n", j, fp->name, fp->type, fp->len, fp->dec);
	}
       fprintf(_clip_dbg_out, "\t%d indexes: current %d\n", wa->rd->ords_opened, wa->rd->curord);
       for (j = 0; j < wa->rd->ords_opened; j++)
	{
	   RDD_ORDER *ip = wa->rd->orders[j];

	   fprintf(_clip_dbg_out, "\t\tno:%d\tname:%s\texpr:%s\tunique:%d\n", j, ip->name, ip->expr, ip->unique);
	}
/*
		fprintf(_clip_dbg_out, "\t%d relations: parent:%s\n", wa->relnum
			,wa->parent ? nullstr(wa->parent->alias) : "");
		for (j = 0; j < wa->relnum; j++)
		{
			DBRelation *rp = wa->relations + j;

			fprintf(_clip_dbg_out, "\t\tno:%d\tchield:%s\texpr:%s\n"
				,j, rp->chield->alias, nullstr(rp->expr));
		}
*/
    }
}
