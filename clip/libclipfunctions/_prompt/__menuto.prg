function __menuTo(choice,_varname)
	local oldcur, colDop, colOsn
	local nChoice:=0, i:=0, j
	local oldChoice,PrevChoice:=-1,lWrap:=set(_SET_WRAP)
	local nKey:=0, flagSeek:=.f.
	local bKeyBlock, oldErrBlock,err
	local _massp,_masshk
	local chr, lenMass:=len(massPrompt)
	local mI, mCol, mChoice ,maxlen:=0
	//parameters choice,_varname
	set("__readvar",_varname)
	oldcur:=setcursor(0)
	colOsn:=colDop:=setcolor()
	colDop:=substr(colDop,at(',',colDop)+1)
	colDop:=substr(colDop,1,at(',',colDop)-1)
	if valtype(choice)=="B"
		oldErrBlock:=errorBlock({ |e| break(e) } )
		begin sequence
			nchoice=eval(choice)
		recover using err
			nchoice:=1
		end sequence
		errorBlock(oldErrBlock)
	else
		nchoice=1
	endif
	if valtype(nchoice)!="N"
		nchoice=1
	endif
	if nchoice==NIL .or. nchoice<=0 .or. nchoice>len(massprompt)
		nchoice=1
	endif
	for i=1 to len(massPrompt)
		maxLen := max(maxLen,len(massPrompt[i][4]))
	next
	for i=1 to len(massPrompt)
		if valtype(massPrompt[i][4])=="C"
			massPrompt[i][4] := padr(massPrompt[i][4],maxLen)
		else
			massPrompt[i][4] := space(maxLen)
		endif
	next
	sayPromptVyd(nChoice,colDop,colOsn)
	do while nKey!=27 .and. nKey!=13
		do while .t.
			nKey := InKey(0, 255) //INKEY_ALL)
			if ( (bKeyBlock:=setkey(nKey))!=NIL )
				_massp=aclone(massprompt)
				_masshk=aclone(masshotkey)
				asize(massprompt,0)
				asize(masshotkey,0)
				eval(bKeyBlock,procname(),procline(),_varname)
				set("__readvar",_varname)
				massprompt=aclone(_massp)
				masshotkey=aclone(_masshk)
			else
				exit
			endif
		enddo
		if mRightDown()
			nKey := 27
			loop
		endif
		dispbegin()
		if mLeftDown() .and. mRow() == massprompt[1][1]
			mChoice := 0
			mCol := mCol()
			for mI=1 to len(massprompt)
				if between(mCol, massprompt[mI][2], massprompt[mI][2]+len(massprompt[mI][3]))
					mChoice := mI
					exit
				endif
			next
			if mChoice > 0
				if nChoice == mChoice
					nKey := 13
				else
					sayPrompt(nChoice,colOsn)
					nChoice:=mChoice
					sayPromptVyd(nChoice,colDop,colOsn)
					nKey := -1
				endif
			endif
		endif
		do case
			case nKey==5 .or. nKey==19
				sayPrompt(nChoice,colOsn)
				nChoice:=iif(nChoice > 1, nChoice-1, iif( lWrap, lenMass, nChoice))
				sayPromptVyd(nChoice,colDop,colOsn)
			case nKey==24 .or. nKey==4
				sayPrompt(nChoice,colOsn)
				nChoice:=iif(nChoice < lenMass, nChoice+1,  iif( lWrap, 1, nChoice))
				sayPromptVyd(nChoice,colDop,colOsn)
			case nKey==1
				sayPrompt(nChoice,colOsn)
				nChoice:=1
				sayPromptVyd(nChoice,colDop,colOsn)
			case nKey==6  //End
				sayPrompt(nChoice,colOsn)
				nChoice:=lenMass
				sayPromptVyd(nChoice,colDop,colOsn)
			case nKey >= 32 .and. nKey <= 259
				oldChoice:=nChoice
				chr:=alltrim(lower(chr(lastKey())))
				i:=1 //i:=iif(nChoice<lenMass,nChoice+1,1)
				do while i<=lenMass//i!=nChoice
					if chr==massHotkey[i][2]
						flagSeek:=.t.
						sayPrompt(nChoice,colOsn)
						nChoice:=i
						sayPromptVyd(nChoice,colDop,colOsn)
						keyboard chr(13)
						exit
					endif
					i++ //i:=(iif(i==lenMass, 1, i+1))
				enddo
		endcase
		dispend()
		PrevChoice:=nChoice
	enddo
	sayPrompt(nChoice,colOsn)
	if nKey==27
		nChoice:=0
	endif
	asize(massPrompt,0)
	asize(massHotkey,0)
	nhotkey:=1
	setcursor(oldcur)
	set("__varname","")
return nChoice
