/* (CODB) commands descriptions */

#ifndef CI__CODB_DEFINED

#define CI__CODB_DEFINED

#command select <classname> ;
		from <dep> ;
                [where <wExpr>] ;
                [order by <xOrder>] ;
                to <varname>;
                => ;
                <varname> := codb_oql_select(<"classname">,<"dep">,<"wExpr">,<"xOrder">)


#endif
