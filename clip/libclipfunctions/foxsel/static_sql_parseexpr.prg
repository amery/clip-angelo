static function sql_parseExpr(self,expr)
	local fexit:=.f.,s , ch, word:="",x,y,ret:={}
        local fs1:=.f., fs2:=.f., fs3:=0, fs4:=0
        local opers:=" ( ) < <= = == >= > % ! != # <> $ ' "+' " '
        local words:=" LIKE NOT .NOT. EXIST ALL ANY SOME BETWEEN AND OR .OR. .AND. XOR .XOR. IN "
        if valtype(expr)!="C"
        	return expr
        endif
        if empty(expr)
        	return {}
        endif
        s:=alltrim(expr)+" "
#ifdef DEBUG_WHERE
        ? "w_s=",s
#endif
        while   !fexit
        	if empty(s) //.and. empty(word)
                	fexit:=.t.
                endif
        	ch:=substr(s,1,1)
                s:=substr(s,2)
#ifdef DEBUG_WHERE
                ? "w_1:",s,":",ch,":",word,":",ret
#endif
                if ! (ch $ opers) .and. ! (word $ opers ) .and. len(s)!=0
                	word+=ch
                        loop
                endif
#ifdef DEBUG_WHERE
                ? "w_2:",s,":",ch,":",word,":",ret
#endif
                if fs3>0  // function expr
                	word+=ch
                        if ch==")"
                        	fs3 --
                        endif
                        if ch=="("
                        	fs3 ++
                        endif
                        if fs3 == 0
                        	aadd(ret,word)
                                word:=""
                        endif
                       	loop
                endif
                if fs4>0  // sub query "select expr"
                        if ch==")"
                        	fs4--
                        endif
                        if ch=="("
                        	fs4++
                        endif
                        if fs4==0
                        	aadd(ret,word)
                               	word:=""
                        else
                		word+=ch
                        endif
                        loop
                endif
                /* end of string */
                if (fs1 .and. ch=='"') .or. (fs2 .and. ch=="'")
                     	aadd(ret,word+ch)
                        word:=""
                        fs1:=fs2:=.f.
                        if len(s)==0
                        	s:=""
                                fExit:=.f.
                        endif
                        loop
		endif
#ifdef DEBUG_WHERE
                ? "w_31:",s,":",ch,":",word,":",ret
#endif
                /* symbol in string */
                if fs1 .or. fs2
                	word+=ch
                        loop
                endif
#ifdef DEBUG_WHERE
                ? "w_32:",s,":",ch,":",word,":",ret
#endif
		if ch=='"'
       			fs1:=.t.
                        if empty(word)
                        	word:=ch
                        	loop
                        endif
                endif
#ifdef DEBUG_WHERE
                ? "w_33:",s,":",ch,":",word,":",ret
#endif
		if ch=="'"
       			fs2:=.t.
                        word:=ch
                        loop
                endif
#ifdef DEBUG_WHERE
                ? "w_4",":",ch,":",word,":",ret
#endif
                /* space between oper symbols "< =", "< >", "< =", "= =" */
                if !empty(word) .and. !empty(ch) .and. (alltrim(word)+ch $ opers)
                       	word=alltrim(word)+ch
                        if !empty(s)
                        	loop
                        endif
                endif
#ifdef DEBUG_WHERE
                ? "w_5",":",ch,":",'"'+word+'"',":",ret
#endif
                if !empty(word) .and. (word $ opers)
                	x:=0
                        word:=alltrim(word)
                	do case
                        	case word=="="
                                	x:=SQL_OPER_EQUAL
                        	case word=="=="
                                	x:=SQL_OPER_EXACTEQUAL
                        	case word=="!="
                                	x:=SQL_OPER_NOTEQUAL
                        	case word=="<>"
                                	x:=SQL_OPER_NOTEQUAL
                        	case word=="#"
                                	x:=SQL_OPER_NOTEQUAL
                        	case word==">"
                                	x:=SQL_OPER_GREAT
                        	case word==">="
                                	x:=SQL_OPER_GREATEQUAL
                        	case word=="<"
                                	x:=SQL_OPER_LESS
                        	case word=="%"
                                	x:=SQL_OPER_PERCENT
                        	case word=="<="
                                	x:=SQL_OPER_LESSEQUAL
                        	case word=="$"
                                	x:=SQL_OPER_IN
                                        fs4:=1
                        	case word=="("
                                	x:=SQL_OPER_GROUPOPEN
                        	case word==")"
                                	x:=SQL_OPER_GROUPCLOSE
                                otherwise
                                	? "What is it in sql query:",word
                        endcase
                        if x==SQL_OPER_GROUPOPEN .and. !empty(ret)
                        	y=atail(ret)
                                if valtype(y)=="N"
                                	// sub query
                                	if y>=SQL_OPER_EXIST .and. y<=SQL_OPER_SOME ;
                                           .or. y==SQL_OPER_IN
                                        	fs4:=1
                                        	word:=ch
                                        	loop
                                        else  // sub group
		                        	aadd(ret,x)
                                        endif

                                else  // call function
                                     	asize(ret,len(ret)-1)
                                     	word:=y+"("+ch
                                       	fs3 ++
                                        loop
                                endif
			else
                        	aadd(ret,x)
                        endif
                        word:=iif(word==ch,"",ch)
			if ch=='"'
       				fs1:=.t.
                        	word:=ch
                        	loop
                	endif
			if ch=="'"
       				fs2:=.t.
                        	word:=ch
                        	loop
                	endif
                        loop
                endif
#ifdef DEBUG_WHERE
                ? "w_6",":",ch,":",word,":",ret
#endif
		y:=" "+upper(alltrim(word))+" "
                if !empty(y) .and. upper(y) $ words
                	word:=alltrim(upper(word))
                        x:=0
                	do case
                        	case word=="NOT"
                                	x:=SQL_OPER_NOT
                        	case word==".NOT."
                                	x:=SQL_OPER_NOT
                        	case word=="AND"
                                	x:=SQL_OPER_AND
                        	case word==".AND."
                                	x:=SQL_OPER_AND
                        	case word=="OR"
                                	x:=SQL_OPER_OR
                        	case word==".OR."
                                	x:=SQL_OPER_OR
                        	case word=="XOR"
                                	x:=SQL_OPER_XOR
                        	case word==".XOR."
                                	x:=SQL_OPER_XOR
                        	case word=="LIKE"
                                	x:=SQL_OPER_LIKE
                        	case word=="BETWEEN"
                                	x:=SQL_OPER_BETWEEN

                        	case word=="EXIST"
                                	x:=SQL_OPER_EXIST
                        	case word=="ALL"
                                	x:=SQL_OPER_ALL
                        	case word=="ANY"
                                	x:=SQL_OPER_ANY
                        	case word=="SOME"
                                	x:=SQL_OPER_SOME
                        	case word=="IN"
                                	x:=SQL_OPER_IN

                                otherwise
                                	fx_sql_error("Not supported constructions:"+word,__LINE__)
                        endcase
                        aadd(ret,x)
                        word:=ch
			if ch=='"'
       				fs1:=.t.
                        	word:=ch
                        	loop
                	endif
			if ch=="'"
       				fs2:=.t.
                        	word:=ch
                        	loop
                	endif
                        loop
                endif
                if !empty(word)
                	aadd(ret,word)
                endif
#ifdef DEBUG_WHERE
                ? "w_7",":",ch,":",word,":",ret
#endif
                word:=ch
        enddo
#ifdef DEBUG_WHERE
        ? fs1,fs2,fs3,fs4
        ? "where=",ret
        quit
#endif
        if fs1 .or. fs2 .or. fs3!=0 .or. fs4!=0
        	fx_sql_error("bad expression in WHERE or HAVING:",__LINE__)
        endif
return ret
