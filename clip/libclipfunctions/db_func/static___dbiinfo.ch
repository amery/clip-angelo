// return structure with info for all aliases
static function __dbiInfo(nInfo)
		local __dbi_names,__dbri_names,__dboi_names,__blob_names
#include "ci_dbinames.ch"
		do switch (nInfo)
			case 1
					return __dbi_names
			case 2
					return __dbri_names
			case 3
					return __dboi_names
			case 4
					return __blob_names
		endswitch
return
