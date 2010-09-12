func TBrowseDB(Lrow,Lcol,Rrow,Rcol)
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"browseDB")
#endif
return TBrowseNew(Lrow,Lcol,Rrow,Rcol,.t.)
