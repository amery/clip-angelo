/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

	This file prints debug informations on stderr
*/
#ifdef DEBUGGING_CLIP
  /* fprintf(stderr, _clic_gettext("Procedure : %s"), __PROC__); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->next->inited : %d\n"), ClipMachineMemory->next->inited);
fprintf(stderr, _clic_gettext("ClipMachineMemory->next->deleted : %d\n"), ClipMachineMemory->next->deleted);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->bp : %s\n"), ClipMachineMemory->bp); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->stack : %s\n"), ClipMachineMemory->fp->stack); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->sp : %s\n"), ClipMachineMemory->fp->sp); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->filename : %s\n"), ClipMachineMemory->fp->filename);
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->line : %d\n"), ClipMachineMemory->fp->line);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->privates : %d\n"), ClipMachineMemory->fp->privates); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->localvars->refcount : %d\n"), ClipMachineMemory->fp->localvars->refcount);
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->localvars->size : %d\n"), ClipMachineMemory->fp->localvars->size);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->localvars->vars : %d\n"), ClipMachineMemory->fp->localvars->vars); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->localvars->names : %s\n"), ClipMachineMemory->fp->localvars->names);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->privates : %d\n"), ClipMachineMemory->fp->privates); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->locals->name : %s\n"), ClipMachineMemory->fp->locals->name); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->locals->vp : %d\n"), ClipMachineMemory->fp->locals->vp); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->statics->name : %s\n"), ClipMachineMemory->fp->statics->name); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->statics->vp : %d\n"), ClipMachineMemory->fp->statics->vp); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->stack : %d\n"), ClipMachineMemory->fp->up->stack); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->sp : %d\n"), ClipMachineMemory->fp->up->sp); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->filename : %s\n"), ClipMachineMemory->fp->up->filename);
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->line : %d\n"), ClipMachineMemory->fp->up->line);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->privates : %d\n"), ClipMachineMemory->fp->up->privates); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->localvars->refcount : %d\n"), ClipMachineMemory->fp->up->localvars->refcount);
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->localvars->size : %d\n"), ClipMachineMemory->fp->up->localvars->size);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->localvars->vars : %d\n"), ClipMachineMemory->fp->up->localvars->vars); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->localvars->names : %s\n"), ClipMachineMemory->fp->up->localvars->names);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->locals->name : %d\n"), ClipMachineMemory->fp->up->locals->name); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->locals->vp : %d\n"), ClipMachineMemory->fp->up->locals->vp); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->statics->name : %d\n"), ClipMachineMemory->fp->up->statics->name); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fp->up->statics->vp : %s\n"), ClipMachineMemory->fp->up->statics->vp); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->argc : %d\n"), ClipMachineMemory->argc);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->trapVar : %s\n"), ClipMachineMemory->trapVar); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->trapTrace->items : %s\n"), ClipMachineMemory->trapTrace->items); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->trapTrace->count : %d\n"), ClipMachineMemory->trapTrace->count); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->publics : %s\n"), ClipMachineMemory->publics); */
/*
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->delete : %d\n"), ClipMachineMemory->publics->delete);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->len : %d\n"), ClipMachineMemory->publics->len);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->clear : %d\n"), ClipMachineMemory->publics->clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->Clear) : %d\n"), ClipMachineMemory->publics->Clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->insert : %d\n"), ClipMachineMemory->publics->insert);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->store : %d\n"), ClipMachineMemory->publics->store);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->fetch : %d\n"), ClipMachineMemory->publics->fetch);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->remove : %d\n"), ClipMachineMemory->publics->remove);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->first : %d\n"), ClipMachineMemory->publics->first);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->next : %d\n"), ClipMachineMemory->publics->next);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->publics->current : %d\n"), ClipMachineMemory->publics->current);
 */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->privates : %s\n"), ClipMachineMemory->privates); */
/*
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->delete : %d\n"), ClipMachineMemory->privates->delete);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->len : %d\n"), ClipMachineMemory->privates->len);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->clear : %d\n"), ClipMachineMemory->privates->clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->Clear) : %d\n"), ClipMachineMemory->privates->Clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->insert : %d\n"), ClipMachineMemory->privates->insert);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->store : %d\n"), ClipMachineMemory->privates->store);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->fetch : %d\n"), ClipMachineMemory->privates->fetch);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->remove : %d\n"), ClipMachineMemory->privates->remove);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->first : %d\n"), ClipMachineMemory->privates->first);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->next : %d\n"), ClipMachineMemory->privates->next);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->privates->current : %d\n"), ClipMachineMemory->privates->current);
 */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces : %s\n"), ClipMachineMemory->spaces); */
/*
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->delete : %d\n"), ClipMachineMemory->spaces->delete);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->len : %d\n"), ClipMachineMemory->spaces->len);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->clear : %d\n"), ClipMachineMemory->spaces->clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->Clear) : %d\n"), ClipMachineMemory->spaces->Clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->insert : %d\n"), ClipMachineMemory->spaces->insert);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->store : %d\n"), ClipMachineMemory->spaces->store);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->fetch : %d\n"), ClipMachineMemory->spaces->fetch);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->remove : %d\n"), ClipMachineMemory->spaces->remove);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->first : %d\n"), ClipMachineMemory->spaces->first);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->next : %d\n"), ClipMachineMemory->spaces->next);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->spaces->current : %d\n"), ClipMachineMemory->spaces->current);
 */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->functions : %s\n"), ClipMachineMemory->functions); */
/*
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->delete : %d\n"), ClipMachineMemory->functions->delete);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->len : %d\n"), ClipMachineMemory->functions->len);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->clear : %d\n"), ClipMachineMemory->functions->clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->Clear) : %d\n"), ClipMachineMemory->functions->Clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->insert : %d\n"), ClipMachineMemory->functions->insert);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->store : %d\n"), ClipMachineMemory->functions->store);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->fetch : %d\n"), ClipMachineMemory->functions->fetch);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->remove : %d\n"), ClipMachineMemory->functions->remove);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->first : %d\n"), ClipMachineMemory->functions->first);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->next : %d\n"), ClipMachineMemory->functions->next);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->functions->current : %d\n"), ClipMachineMemory->functions->current);
 */
fprintf(stderr, _clic_gettext("ClipMachineMemory->main_func_level : %d\n"), ClipMachineMemory->main_func_level);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->dlls->items : %d\n"), ClipMachineMemory->dlls->items); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->dlls->count : %d\n"), ClipMachineMemory->dlls->count); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits : %d\n"), ClipMachineMemory->exits); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->refCount : %d\n"), ClipMachineMemory->exits->file->refCount);
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->body : %s\n"), ClipMachineMemory->exits->file->body);
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->mem : %s\n"), ClipMachineMemory->exits->file->mem);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->bodySize : %d\n"), ClipMachineMemory->exits->file->bodySize); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->bodyMem : %d\n"), ClipMachineMemory->exits->file->bodyMem);
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->name : %s\n"), ClipMachineMemory->exits->file->name);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->statics : %s\n"), ClipMachineMemory->exits->file->statics); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->statics->staticsMem : %d\n"), ClipMachineMemory->exits->file->statics->staticsMem); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->statics->nstatics : %d\n"), ClipMachineMemory->exits->file->statics->nstatics); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->statics->pos : %d\n"), ClipMachineMemory->exits->file->statics->pos); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->statics->end : %d\n"), ClipMachineMemory->exits->file->statics->end); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->statics->cargo : %s\n"), ClipMachineMemory->exits->file->statics->cargo); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->hash_names->num : %d\n"), ClipMachineMemory->exits->file->hash_names->num);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->hash_names->buckets : %s\n"), ClipMachineMemory->exits->file->hash_names->buckets); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->hash_names->mem : %s\n"), ClipMachineMemory->exits->file->hash_names->mem);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->staticDefs->name : %s\n"), ClipMachineMemory->exits->file->staticDefs->name); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->staticDefs->vp : %s\n"), ClipMachineMemory->exits->file->staticDefs->vp); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->file->fd : %d\n"), ClipMachineMemory->exits->file->fd);
fprintf(stderr, _clic_gettext("ClipMachineMemory->exits->func : %s\n"), ClipMachineMemory->exits->func);
fprintf(stderr, _clic_gettext("ClipMachineMemory->nexits : %d\n"), ClipMachineMemory->nexits);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->cexits : %d\n"), ClipMachineMemory->cexits); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->ncexits : %d\n"), ClipMachineMemory->ncexits);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->keys : %d\n"), ClipMachineMemory->keys); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->corba : %d\n"), ClipMachineMemory->corba);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->dbdrivers->id : %c\n"), ClipMachineMemory->dbdrivers->id); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->dbdrivers->data : %c\n"), ClipMachineMemory->dbdrivers->data); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->dbdrivers->idx : %c\n"), ClipMachineMemory->dbdrivers->idx); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->dbdrivers->memo : %c\n"), ClipMachineMemory->dbdrivers->memo); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->ndbdrivers : %d\n"), ClipMachineMemory->ndbdrivers); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->id : %s\n"), ClipMachineMemory->data_drivers->id); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->suff : %s\n"), ClipMachineMemory->data_drivers->suff); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->desc : %s\n"), ClipMachineMemory->data_drivers->desc); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->version : %d\n"), ClipMachineMemory->data_drivers->version); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->dbfsig : %s\n"), ClipMachineMemory->data_drivers->dbfsig); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->dbfwithmemosig : %s\n"), ClipMachineMemory->data_drivers->dbfwithmemosig); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->open : %s\n"), ClipMachineMemory->data_drivers->open); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->close : %s\n"), ClipMachineMemory->data_drivers->close); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->create : %s\n"), ClipMachineMemory->data_drivers->create); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->pack : %s\n"), ClipMachineMemory->data_drivers->pack); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->zap : %s\n"), ClipMachineMemory->data_drivers->zap); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->lastrec : %s\n"), ClipMachineMemory->data_drivers->lastrec); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->deleted : %s\n"), ClipMachineMemory->data_drivers->deleted); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->lupdate : %s\n"), ClipMachineMemory->data_drivers->lupdate); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->setstruct : %s\n"), ClipMachineMemory->data_drivers->setstruct); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->gotop : %s\n"), ClipMachineMemory->data_drivers->gotop); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->gobottom : %s\n"), ClipMachineMemory->data_drivers->gobottom); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->next : %s\n"), ClipMachineMemory->data_drivers->next); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->prev : %s\n"), ClipMachineMemory->data_drivers->prev); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->go : %s\n"), ClipMachineMemory->data_drivers->go); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->rawgo : %s\n"), ClipMachineMemory->data_drivers->rawgo); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->rlock : %s\n"), ClipMachineMemory->data_drivers->rlock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->flock : %s\n"), ClipMachineMemory->data_drivers->flock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->ulock : %s\n"), ClipMachineMemory->data_drivers->ulock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->rlocked : %s\n"), ClipMachineMemory->data_drivers->rlocked); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->forlock : %s\n"), ClipMachineMemory->data_drivers->forlock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->_wlock : %s\n"), ClipMachineMemory->data_drivers->_wlock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->_rlock : %s\n"), ClipMachineMemory->data_drivers->_rlock);  */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->_ulock : %s\n"), ClipMachineMemory->data_drivers->_ulock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->append : %s\n"), ClipMachineMemory->data_drivers->append); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->getrecord : %s\n"), ClipMachineMemory->data_drivers->getrecord); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->getvalue : %s\n"), ClipMachineMemory->data_drivers->getvalue); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->setrecord : %s\n"), ClipMachineMemory->data_drivers->setrecord); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->setvalue : %s\n"), ClipMachineMemory->data_drivers->setvalue); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->getmemoid : %s\n"), ClipMachineMemory->data_drivers->getmemoid); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->setmemoid : %s\n"), ClipMachineMemory->data_drivers->setmemoid); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->delete : %s\n"), ClipMachineMemory->data_drivers->delete); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->recall : %s\n"), ClipMachineMemory->data_drivers->recall); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->info : %s\n"), ClipMachineMemory->data_drivers->info); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->verify : %s\n"), ClipMachineMemory->data_drivers->verify); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->data_drivers->calcfiltlist : %s\n"), ClipMachineMemory->data_drivers->calcfiltlist); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->ndata_drivers : %d\n"), ClipMachineMemory->ndata_drivers); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->id : %s\n"), ClipMachineMemory->idx_drivers->id); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->suff : %s\n"), ClipMachineMemory->idx_drivers->suff); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->sing_suff : %s\n"), ClipMachineMemory->idx_drivers->sing_suff); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->desc : %s\n"), ClipMachineMemory->idx_drivers->desc); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->version : %d\n"), ClipMachineMemory->idx_drivers->version); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ismulti : %s\n"), ClipMachineMemory->idx_drivers->ismulti); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->open : %d\n"), ClipMachineMemory->idx_drivers->open); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->close : %d\n"), ClipMachineMemory->idx_drivers->close); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->create : %d\n"), ClipMachineMemory->idx_drivers->create); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->reindex : %d\n"), ClipMachineMemory->idx_drivers->reindex); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->zap : %d\n"), ClipMachineMemory->idx_drivers->zap); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->gotop : %d\n"), ClipMachineMemory->idx_drivers->gotop); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->gobottom : %d\n"), ClipMachineMemory->idx_drivers->gobottom); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->next : %d\n"), ClipMachineMemory->idx_drivers->next); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->prev : %d\n"), ClipMachineMemory->idx_drivers->prev); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->seek : %d\n"), ClipMachineMemory->idx_drivers->seek); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->gotokey : %d\n"), ClipMachineMemory->idx_drivers->gotokey); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->descend : %d\n"), ClipMachineMemory->idx_drivers->descend); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->buildtree : %d\n"), ClipMachineMemory->idx_drivers->buildtree); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->wildskip : %d\n"), ClipMachineMemory->idx_drivers->wildskip); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->addkey : %d\n"), ClipMachineMemory->idx_drivers->addkey); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->delkey : %d\n"), ClipMachineMemory->idx_drivers->delkey); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->destroy : %d\n"), ClipMachineMemory->idx_drivers->destroy); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->formatkey : %d\n"), ClipMachineMemory->idx_drivers->formatkey); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->keyno : %d\n"), ClipMachineMemory->idx_drivers->keyno); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->lastkey : %d\n"), ClipMachineMemory->idx_drivers->lastkey); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->info : %d\n"), ClipMachineMemory->idx_drivers->info); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->keyvalue : %d\n"), ClipMachineMemory->idx_drivers->keyvalue); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->setscope : %d\n"), ClipMachineMemory->idx_drivers->setscope); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_create : %d\n"), ClipMachineMemory->idx_drivers->ii_create); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_createtag : %d\n"), ClipMachineMemory->idx_drivers->ii_createtag); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_open : %d\n"), ClipMachineMemory->idx_drivers->ii_open); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_opentag : %d\n"), ClipMachineMemory->idx_drivers->ii_opentag); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_addkey : %d\n"), ClipMachineMemory->idx_drivers->ii_addkey); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_delkey : %d\n"), ClipMachineMemory->idx_drivers->ii_delkey); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_gotop : %d\n"), ClipMachineMemory->idx_drivers->ii_gotop); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_gobottom : %d\n"), ClipMachineMemory->idx_drivers->ii_gobottom); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_id : %d\n"), ClipMachineMemory->idx_drivers->ii_id); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_key : %d\n"), ClipMachineMemory->idx_drivers->ii_key); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_next : %d\n"), ClipMachineMemory->idx_drivers->ii_next); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->ii_prev : %d\n"), ClipMachineMemory->idx_drivers->ii_prev); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->calcfiltlist : %d\n"), ClipMachineMemory->idx_drivers->calcfiltlist); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->_rlock : %d\n"), ClipMachineMemory->idx_drivers->_rlock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->_wlock : %d\n"), ClipMachineMemory->idx_drivers->_wlock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->_ulock : %d\n"), ClipMachineMemory->idx_drivers->_ulock); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->nidx_drivers : %d\n"), ClipMachineMemory->nidx_drivers); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->memo_drivers->id : %s\n"), ClipMachineMemory->memo_drivers->id); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->memo_drivers->suff : %s\n"), ClipMachineMemory->memo_drivers->suff); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->memo_drivers->desc : %s\n"), ClipMachineMemory->memo_drivers->desc); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->version : %d\n"), ClipMachineMemory->idx_drivers->version); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->create : %d\n"), ClipMachineMemory->idx_drivers->create); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->pack : %d\n"), ClipMachineMemory->idx_drivers->pack); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->zap : %d\n"), ClipMachineMemory->idx_drivers->zap); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->open : %d\n"), ClipMachineMemory->idx_drivers->open); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->close : %d\n"), ClipMachineMemory->idx_drivers->close); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->getvalue : %d\n"), ClipMachineMemory->idx_drivers->getvalue); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->setvalue : %d\n"), ClipMachineMemory->idx_drivers->setvalue); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->getvchar : %d\n"), ClipMachineMemory->idx_drivers->getvchar); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->setvchar : %d\n"), ClipMachineMemory->idx_drivers->setvchar); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->getvalue64 : %d\n"), ClipMachineMemory->idx_drivers->getvalue64); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->setvalue64 : %d\n"), ClipMachineMemory->idx_drivers->setvalue64); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->getvchar64 : %d\n"), ClipMachineMemory->idx_drivers->getvchar64); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->setvchar64 : %d\n"), ClipMachineMemory->idx_drivers->setvchar64); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->idx_drivers->info : %d\n"), ClipMachineMemory->idx_drivers->info); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->nmemo_drivers : %d\n"), ClipMachineMemory->nmemo_drivers); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->def_data_driver : %s\n"), ClipMachineMemory->def_data_driver);
fprintf(stderr, _clic_gettext("ClipMachineMemory->def_idx_driver : %s\n"), ClipMachineMemory->def_idx_driver);
fprintf(stderr, _clic_gettext("ClipMachineMemory->def_memo_driver : %s\n"), ClipMachineMemory->def_memo_driver);
fprintf(stderr, _clic_gettext("ClipMachineMemory->def_db_driver : %s\n"), ClipMachineMemory->def_db_driver);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->sqldrivers->id : %s\n"), ClipMachineMemory->sqldrivers->id); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->sqldrivers->name : %s\n"), ClipMachineMemory->sqldrivers->name); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->sqldrivers->desc : %s\n"), ClipMachineMemory->sqldrivers->desc); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->sqldrivers->connect : %s\n"), ClipMachineMemory->sqldrivers->connect); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->nsqldrivers : %d\n"), ClipMachineMemory->nsqldrivers); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks : %s\n"), ClipMachineMemory->tasklocks); */
/*
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->delete : %d\n"), ClipMachineMemory->tasklocks->delete);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->len : %d\n"), ClipMachineMemory->tasklocks->len);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->clear : %d\n"), ClipMachineMemory->tasklocks->clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->Clear) : %d\n"), ClipMachineMemory->tasklocks->Clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->insert : %d\n"), ClipMachineMemory->tasklocks->insert);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->store : %d\n"), ClipMachineMemory->tasklocks->store);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->fetch : %d\n"), ClipMachineMemory->tasklocks->fetch);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->remove : %d\n"), ClipMachineMemory->tasklocks->remove);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->first : %d\n"), ClipMachineMemory->tasklocks->first);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->next : %d\n"), ClipMachineMemory->tasklocks->next);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->tasklocks->current : %d\n"), ClipMachineMemory->tasklocks->current);
 */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens : %s\n"), ClipMachineMemory->fileopens); */
/*
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->delete : %d\n"), ClipMachineMemory->fileopens->delete);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->len : %d\n"), ClipMachineMemory->fileopens->len);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->clear : %d\n"), ClipMachineMemory->fileopens->clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->Clear) : %d\n"), ClipMachineMemory->fileopens->Clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->insert : %d\n"), ClipMachineMemory->fileopens->insert);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->store : %d\n"), ClipMachineMemory->fileopens->store);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->fetch : %d\n"), ClipMachineMemory->fileopens->fetch);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->remove : %d\n"), ClipMachineMemory->fileopens->remove);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->first : %d\n"), ClipMachineMemory->fileopens->first);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->next : %d\n"), ClipMachineMemory->fileopens->next);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->fileopens->current : %d\n"), ClipMachineMemory->fileopens->current);
 */
fprintf(stderr, _clic_gettext("ClipMachineMemory->neterr : %d\n"), ClipMachineMemory->neterr);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases : %d\n"), ClipMachineMemory->aliases); */
/*
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->delete : %d\n"), ClipMachineMemory->aliases->delete);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->len : %d\n"), ClipMachineMemory->aliases->len);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->clear : %d\n"), ClipMachineMemory->aliases->clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->Clear) : %d\n"), ClipMachineMemory->aliases->Clear);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->insert : %d\n"), ClipMachineMemory->aliases->insert);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->store : %d\n"), ClipMachineMemory->aliases->store);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->fetch : %d\n"), ClipMachineMemory->aliases->fetch);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->remove : %d\n"), ClipMachineMemory->aliases->remove);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->first : %d\n"), ClipMachineMemory->aliases->first);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->next : %d\n"), ClipMachineMemory->aliases->next);
	fprintf(stderr, _clic_gettext("ClipMachineMemory->aliases->current : %d\n"), ClipMachineMemory->aliases->current);
 */
	/* fprintf(stderr, _clic_gettext("areas->items : %s\n"), ClipMachineMemory->areas->items); */
fprintf(stderr, _clic_gettext("areas->count : %d\n"), ClipMachineMemory->areas->count);

	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->areaStack->items : %s\n"), ClipMachineMemory->areaStack->items); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->areaStack->count : %d\n"), ClipMachineMemory->areaStack->count);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->driver : %s\n"), ClipMachineMemory->driver); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->hours : %d\n"), ClipMachineMemory->hours);
fprintf(stderr, _clic_gettext("ClipMachineMemory->seconds : %d\n"), ClipMachineMemory->seconds);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->flags : %d\n"), ClipMachineMemory->flags); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->flags1 : %d\n"), ClipMachineMemory->flags1); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->optimizelevel : %d\n"), ClipMachineMemory->optimizelevel);
fprintf(stderr, _clic_gettext("ClipMachineMemory->m6_error : %d\n"), ClipMachineMemory->m6_error);
fprintf(stderr, _clic_gettext(" ClipMachineMemory->index_buffer_limit : %d\n"), ClipMachineMemory->index_buffer_limit);
fprintf(stderr, _clic_gettext("ClipMachineMemory->lockstyle : %d\n"), ClipMachineMemory->lockstyle);
fprintf(stderr, _clic_gettext("ClipMachineMemory->autorder : %d\n"), ClipMachineMemory->autorder);
fprintf(stderr, _clic_gettext("ClipMachineMemory->date_format : %s\n"), ClipMachineMemory->date_format);
fprintf(stderr, _clic_gettext("ClipMachineMemory->decimals : %d\n"), ClipMachineMemory->decimals);
fprintf(stderr, _clic_gettext("ClipMachineMemory->fileCreateMode : %d\n"), ClipMachineMemory->fileCreateMode);
fprintf(stderr, _clic_gettext("ClipMachineMemory->dirCreateMode : %d\n"), ClipMachineMemory->dirCreateMode);
fprintf(stderr, _clic_gettext("ClipMachineMemory->epoch : %d\n"), ClipMachineMemory->epoch);
fprintf(stderr, _clic_gettext("ClipMachineMemory->path : %s\n"), ClipMachineMemory->path);
fprintf(stderr, _clic_gettext("ClipMachineMemory->defaul : %s\n"), ClipMachineMemory->defaul);
fprintf(stderr, _clic_gettext("ClipMachineMemory->typeahead : %d\n"), ClipMachineMemory->typeahead);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->kbdbuf : %d\n"), ClipMachineMemory->kbdbuf); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->kbdptr : %d\n"), ClipMachineMemory->kbdptr); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->lastkey : %d\n"), ClipMachineMemory->lastkey);
fprintf(stderr, _clic_gettext("ClipMachineMemory->cursor : %d\n"), ClipMachineMemory->cursor);
fprintf(stderr, _clic_gettext("ClipMachineMemory->altfile : %s\n"), ClipMachineMemory->altfile);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->out : %s\n"), ClipMachineMemory->out); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->alt : %s\n"), ClipMachineMemory->alt); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->extrafile : %s\n"), ClipMachineMemory->extrafile);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->extra : %s\n"), ClipMachineMemory->extra); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->prfile : %s\n"), ClipMachineMemory->prfile);
fprintf(stderr, _clic_gettext("ClipMachineMemory->real_prfile : %s\n"), ClipMachineMemory->real_prfile);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->printer : %s\n"), ClipMachineMemory->printer); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->rootpath : %s\n"), ClipMachineMemory->rootpath);
fprintf(stderr, _clic_gettext("ClipMachineMemory->margin : %d\n"), ClipMachineMemory->margin);
fprintf(stderr, _clic_gettext("ClipMachineMemory->delimchars : %s\n"), ClipMachineMemory->delimchars);
fprintf(stderr, _clic_gettext("ClipMachineMemory->msgline : %d\n"), ClipMachineMemory->msgline);
fprintf(stderr, _clic_gettext("ClipMachineMemory->notcancel : %d\n"), ClipMachineMemory->notcancel);
fprintf(stderr, _clic_gettext("ClipMachineMemory->eventmask : %d\n"), ClipMachineMemory->eventmask);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->mem : %s\n"), ClipMachineMemory->screen->mem); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->chars : %s\n"), ClipMachineMemory->screen->chars); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->colors : %s\n"), ClipMachineMemory->screen->colors); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->attrs : %s\n"), ClipMachineMemory->screen->attrs); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->touched : %d\n"), ClipMachineMemory->screen->touched); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->lnums : %d\n"), ClipMachineMemory->screen->lnums); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->x : %d\n"), ClipMachineMemory->screen->x); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->y : %d\n"), ClipMachineMemory->screen->y); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->beeps : %d\n"), ClipMachineMemory->screen->beeps); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen->cursor : %d\n"), ClipMachineMemory->screen->cursor); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->Lines : %d\n"), ClipMachineMemory->screen_base->Lines); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->Columns : %d\n"), ClipMachineMemory->screen_base->Columns); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->clear_on_exit : %d\n"), ClipMachineMemory->screen_base->clear_on_exit); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->fd : %d\n"), ClipMachineMemory->screen_base->fd); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->terminalName : %s\n"), ClipMachineMemory->screen_base->terminalName); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->realScreen : %d\n"), ClipMachineMemory->screen_base->realScreen); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_present : %d\n"), ClipMachineMemory->screen_base->mouse_present); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_y : %d\n"), ClipMachineMemory->screen_base->mouse_y); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_x : %d\n"), ClipMachineMemory->screen_base->mouse_x); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_visible : %d\n"), ClipMachineMemory->screen_base->mouse_visible); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_auto_visible : %d\n"), ClipMachineMemory->screen_base->mouse_auto_visible); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_top : %d\n"), ClipMachineMemory->screen_base->mouse_top); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_bottom : %d\n"), ClipMachineMemory->screen_base->mouse_bottom); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_left : %d\n"), ClipMachineMemory->screen_base->mouse_left); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_right : %d\n"), ClipMachineMemory->screen_base->mouse_right); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_buttons : %d\n"), ClipMachineMemory->screen_base->mouse_buttons); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_dclick_speed : %d\n"), ClipMachineMemory->screen_base->mouse_dclick_speed); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_clicks_left : %d\n"), ClipMachineMemory->screen_base->mouse_clicks_left); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_clicks_right : %d\n"), ClipMachineMemory->screen_base->mouse_clicks_right); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_clicks_middle : %d\n"), ClipMachineMemory->screen_base->mouse_clicks_middle); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->mouse_driver : %s\n"), ClipMachineMemory->screen_base->mouse_driver); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->pg_chars : %d\n"), ClipMachineMemory->screen_base->pg_chars); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->screen_base->data : %d\n"), ClipMachineMemory->screen_base->data); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->fullscreen : %d\n"), ClipMachineMemory->fullscreen);
fprintf(stderr, _clic_gettext("ClipMachineMemory->update : %d\n"), ClipMachineMemory->update);
fprintf(stderr, _clic_gettext("ClipMachineMemory->inkey : %d\n"), ClipMachineMemory->inkey);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->standard : %d\n"), ClipMachineMemory->attr->standard); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->enhanced : %d\n"), ClipMachineMemory->attr->enhanced); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->border : %d\n"), ClipMachineMemory->attr->border); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->background : %d\n"), ClipMachineMemory->attr->background); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->unselected : %d\n"), ClipMachineMemory->attr->unselected); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->u1 : %d\n"), ClipMachineMemory->attr->u1); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->u2 : %d\n"), ClipMachineMemory->attr->u2); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->u3 : %d\n"), ClipMachineMemory->attr->u3); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->u4 : %d\n"), ClipMachineMemory->attr->u4); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->attr->u5 : %d\n"), ClipMachineMemory->attr->u5); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->colorSelect : %d\n"), ClipMachineMemory->colorSelect);
fprintf(stderr, _clic_gettext("ClipMachineMemory->prow : %d\n"), ClipMachineMemory->prow);
fprintf(stderr, _clic_gettext("ClipMachineMemory->pcol : %d\n"), ClipMachineMemory->pcol);
fprintf(stderr, _clic_gettext("ClipMachineMemory->buf : %s\n"), ClipMachineMemory->buf);
fprintf(stderr, _clic_gettext("ClipMachineMemory->buflen : %d\n"), ClipMachineMemory->buflen);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->history->items : %s\n"), ClipMachineMemory->history->items); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->history->count : %d\n"), ClipMachineMemory->history->count); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->store : %d\n"), ClipMachineMemory->store); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->errorblock : %d\n"), ClipMachineMemory->errorblock); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->errorlevel : %d\n"), ClipMachineMemory->errorlevel);
fprintf(stderr, _clic_gettext("ClipMachineMemory->noerrblock : %d\n"), ClipMachineMemory->noerrblock);
fprintf(stderr, _clic_gettext("ClipMachineMemory->syserr : %s\n"), ClipMachineMemory->syserr);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->rect->top : %d\n"), ClipMachineMemory->windows->rect->top); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->rect->bottom : %d\n"), ClipMachineMemory->windows->rect->bottom); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->rect->left : %d\n"), ClipMachineMemory->windows->rect->left); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->rect->right : %d\n"), ClipMachineMemory->windows->rect->right); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->format->top : %d\n"), ClipMachineMemory->windows->format->top); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->format->bottom : %d\n"), ClipMachineMemory->windows->format->bottom); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->format->left : %d\n"), ClipMachineMemory->windows->format->left); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->format->right : %d\n"), ClipMachineMemory->windows->format->right); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->no : %d\n"), ClipMachineMemory->windows->no);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->save : %s\n"), ClipMachineMemory->windows->save); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->windows->shadow : %d\n"), ClipMachineMemory->windows->shadow);
fprintf(stderr, _clic_gettext("ClipMachineMemory->wnum : %d\n"), ClipMachineMemory->wnum);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->wboard->top : %d\n"), ClipMachineMemory->wboard->top); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->wboard->bottom : %d\n"), ClipMachineMemory->wboard->bottom); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->wboard->left : %d\n"), ClipMachineMemory->wboard->left); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->wboard->right : %d\n"), ClipMachineMemory->wboard->right); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->wshadow : %d\n"), ClipMachineMemory->wshadow);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->container->items->item : %s\n"), ClipMachineMemory->container->items->item); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->container->items->key : %d\n"), ClipMachineMemory->container->items->key);
fprintf(stderr, _clic_gettext("ClipMachineMemory->container->items->type : %d\n"), ClipMachineMemory->container->items->type);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->container->items->destroy : %s\n"), ClipMachineMemory->container->items->destroy); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->container->len : %d\n"), ClipMachineMemory->container->len);
fprintf(stderr, _clic_gettext("ClipMachineMemory->container->key : %d\n"), ClipMachineMemory->container->key);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->stack : %s\n"), ClipMachineMemory->inMacro->stack); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->sp : %s\n"), ClipMachineMemory->inMacro->sp); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->filename : %s\n"), ClipMachineMemory->inMacro->filename);
fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->line : %d\n"), ClipMachineMemory->inMacro->line);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->privates : %d\n"), ClipMachineMemory->inMacro->privates); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->localvars->refcount : %d\n"), ClipMachineMemory->inMacro->localvars->refcount);
fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->localvars->size : %d\n"), ClipMachineMemory->inMacro->localvars->size);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->localvars->vars : %s\n"), ClipMachineMemory->inMacro->localvars->vars); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->localvars->names : %s\n"), ClipMachineMemory->inMacro->localvars->names);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->locals->name : %s\n"), ClipMachineMemory->inMacro->locals->name); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->locals->vp : %s\n"), ClipMachineMemory->inMacro->locals->vp); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->statics->name : %d\n"), ClipMachineMemory->inMacro->statics->name); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->inMacro->statics->vp : %d\n"), ClipMachineMemory->inMacro->statics->vp); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->hashnames : %d\n"), ClipMachineMemory->hashnames); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->print_deep : %d\n"), ClipMachineMemory->print_deep);
fprintf(stderr, _clic_gettext("ClipMachineMemory->print_max_deep : %d\n"), ClipMachineMemory->print_max_deep);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->prntbl : %d\n"), ClipMachineMemory->prntbl); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->prn_xlat : %d\n"), ClipMachineMemory->prn_xlat); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->term_xlat : %d\n"), ClipMachineMemory->term_xlat); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->fields : %d\n"), ClipMachineMemory->fields); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->obuf : %s\n"), ClipMachineMemory->obuf); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->ebuf : %s\n"), ClipMachineMemory->ebuf); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->obj : %s\n"), ClipMachineMemory->obj); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->profiler : %d\n"), ClipMachineMemory->profiler); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->hash : %d\n"), ClipMachineMemory->pbucket->hash); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->procname : %s\n"), ClipMachineMemory->pbucket->procname);
fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->filename : %s\n"), ClipMachineMemory->pbucket->filename);
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->self->tv_sec : %d\n"), ClipMachineMemory->pbucket->self->tv_sec); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->self->tv_usec : %d\n"), ClipMachineMemory->pbucket->self->tv_usec); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->start->tv_sec : %d\n"), ClipMachineMemory->pbucket->start->tv_sec); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->start->tv_usec : %d\n"), ClipMachineMemory->pbucket->start->tv_usec); */
	/* fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->callno : %d\n"), ClipMachineMemory->pbucket->callno); */
fprintf(stderr, _clic_gettext("ClipMachineMemory->pbucket->started : %d\n"), ClipMachineMemory->pbucket->started);
#endif
