Static Function dv_Parameter ( paParameters)
	Local ln01ii := 0
	Local ln02ii := 0
	Local lsCurrentParameter := ""
	Local lnCurValue := ""
	Local lnNewValue := ""
	ln01ii := 1
	Do While ln01ii <= Len( paParameters)
		lsCurrentParameter := paParameters[ ln01ii ]
		Do Case
		Case lsCurrentParameter == "-a"
			p_bAutoMemVar := .T.
		Case SubStr( lsCurrentParameter, 1, 12 ) == "--auto-memvar"
			p_bAutoMemVar := .T.
			If ( SubStr( lsCurrentParameter, 13, 1 ) == "=" )
				lnCurValue := Val( SubStr( lsCurrentParameter, 14 ) )
				If ( lnCurValue > 1 ) .or. ( lnCurValue < 0)
					aAdd( p_aDevilError, PARAM_ERROR_AUTO_MEMVAR )
					p_bStop := .T.
				Else
					p_bAutoMemVar := iIf( lnCurValue == 1, .T., .F. )
				EndIf
			EndIf
		Case lsCurrentParameter == "-b"
			p_bDebug := .T.
		Case lsCurrentParameter == "-c"
			p_bCompile := .T.
		Case lsCurrentParameter == "--compile"
			p_bCompile := .T.
		Case lsCurrentParameter == "-C"
			p_bShowCommand := .T.
		Case lsCurrentParameter == "--show-command"
			p_bShowCommand := .T.
		Case lsCurrentParameter == "-d"
			p_sOutDir := AllTrim( SubStr( lsCurrentParameter, 3 ))
		Case SubStr( lsCurrentParameter, 1, 8) == "--outdir"
			If ( Len( lsCurrentParameter) > 8 )
				If ( SubStr( lsCurrentParameter, 9, 1 ) == "=" )
					lnCurValue := Val( SubStr( lsCurrentParameter, 10 ) )
					p_sOutDir := AllTrim( lnCurValue )
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_OUTDIR )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-D"
			If ( Len( lsCurrentParameter) > 2 )
				lsCurrentParameter := SubStr(lsCurrentParameter, 3)
				If ( "=" $ lsCurrentParameter)
					ln02ii := At( "=", lsCurrentParameter )
					lnCurValue := SubStr(lsCurrentParameter, 1, ln02ii - 1)
					aAdd( p_aDefine, {lnCurValue, SubStr(lsCurrentParameter, ln02ii + 1 )})
				Else
					aAdd( p_aDefine, {AllTrim( SubStr( lsCurrentParameter, 3 ), "")})
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_D )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 8) == "--define"
			If ( Len( lsCurrentParameter) > 8 )
				If SubStr( lsCurrentParameter, 9, 1 ) == "="
					lsCurrentParameter := SubStr(lsCurrentParameter, 10)
					If ( "=" $ lsCurrentParameter)
						ln02ii := At( "=", lsCurrentParameter )
						lnCurValue := SubStr(lsCurrentParameter, 1, ln02ii - 1)
						aAdd( p_aDefine, {lnCurValue, SubStr(lsCurrentParameter, ln02ii + 1 )})
					Else
						aAdd( p_aDefine, lsCurrentParameter )
					EndIf
				Else
					aAdd( p_aDevilError, PARAM_ERROR_DEFINE )
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_DEFINE )
			EndIf
		Case lsCurrentParameter == "-e"
			p_bExec := .T.
		Case lsCurrentParameter == "--exec"
			p_bExec := .T.
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-E"
			If ( Len( lsCurrentParameter) > 2 )
				lsCurrentParameter := SubStr(lsCurrentParameter, 3)
				If ( "=" $ lsCurrentParameter)
					ln02ii := At( "=", lsCurrentParameter )
					lnCurValue := SubStr(lsCurrentParameter, 1, ln02ii - 1)
					aAdd( p_aVariables, {lnCurValue, SubStr(lsCurrentParameter, ln02ii + 1 )})
				Else
					aAdd( p_aDevilError, PARAM_ERROR_E )
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_E )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 13) == "--environment"
			If ( Len( lsCurrentParameter) > 13 )
				If SubStr( lsCurrentParameter, 14, 1 ) == "="
					lsCurrentParameter := SubStr(lsCurrentParameter, 15)
					If ( "=" $ lsCurrentParameter)
						ln02ii := At( "=", lsCurrentParameter )
						lnCurValue := SubStr(lsCurrentParameter, 1, ln02ii - 1)
						aAdd( p_aVariables, {lnCurValue, SubStr(lsCurrentParameter, ln02ii + 1 )})
					Else
						aAdd( p_aDevilError, PARAM_ERROR_ENVIRONMENT  )
					EndIf
				Else
					aAdd( p_aDevilError, PARAM_ERROR_ENVIRONMENT )
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_ENVIRONMENT )
			EndIf
		Case lsCurrentParameter == "-g"
			p_bDebug := .T.
		Case SubStr( lsCurrentParameter, 1, 7) == "--debug"
			p_bDebug := .T.
			If ( Len( lsCurrentParameter) > 7 )
				If ( SubStr( lsCurrentParameter, 8, 1 ) == "=" )
					lnCurValue := Val( SubStr( lsCurrentParameter, 9 ) )
					If ( lnCurValue > 1 ) .or. ( lnCurValue < 0)
						aAdd( p_aDevilError, PARAM_ERROR_DEBUG )
						p_bStop := .T.
					Else
						p_bDebug := iIf( p_bDebug == 1, .T., .F. )
					EndIf
				EndIf
			EndIf
		Case lsCurrentParameter == "-h" .or. lsCurrentParameter == "-help"
			*PARAM_h
			quit
		Case lsCurrentParameter == "-H" .or.	lsCurrentParameter == "-help-environment"
			*PARAM_H
			Quit
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-I"
			If ( Len( lsCurrentParameter) > 2 )
				aAdd( p_aInclude, SubStr( lsCurrentParameter, 3 ))
			Else
				aAdd( p_aInclude, paParameters[ ++ln01ii ] )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 14) == "--include-path"
			If ( Len( lsCurrentParameter) > 14 )
				If ( SubStr( lsCurrentParameter, 15, 1 ) == "=" )
					aAdd( p_aInclude, SubStr( lsCurrentParameter, 16 ))
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_INCLUDE_PATH )
			EndIf
		Case lsCurrentParameter == "-l"
			p_bcCode := .T.
		Case SubStr( lsCurrentParameter == "-l" ) .and. Len(lsCurrentParameter) > 2
			aAdd ( p_aLibrary, SubStr(lsCurrentParameter, 2 ) )
		Case lsCurrentParameter == "--c-pcode"
			p_bcCode := .T.
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-L"
			If ( Len( lsCurrentParameter) > 2 )
				aAdd( p_aLibrary, SubStr( lsCurrentParameter, 2 ))
			Else
				aAdd( p_aLibrary, paParameters[ ++ln01ii ] )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 14) == "--library-path"
			If ( Len( lsCurrentParameter) > 14 )
				If ( SubStr( lsCurrentParameter, 15, 1 ) == "=" )
					aAdd( p_aLibrary, SubStr( lsCurrentParameter, 16 ))
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_LIBRARY_PATH )
			EndIf
		Case lsCurrentParameter == "-M"
			p_bMain := .T.
		Case lsCurrentParameter == "--main"
			p_bMain := .T.
		Case lsCurrentParameter == "-n"
			p_bNoMain := .T.
		Case lsCurrentParameter == "--nomain"
			p_bNoMain := .T.
		Case lsCurrentParameter == "-N"
			p_bNames := .T.
		Case lsCurrentParameter == "--names"
			p_bNames := .T.
		Case lsCurrentParameter == "-o"
			If ( Len( lsCurrentParameter) > 2 )
				p_sOutput := AllTrim( SubStr( lsCurrentParameter, 3 ))
			Else
				p_sOutput := paParameters[ ++ln01ii ]
			EndIf
		Case SubStr( lsCurrentParameter, 1, 8) == "--output"
			If ( Len( lsCurrentParameter) > 8 )
				If ( SubStr( lsCurrentParameter, 9, 1 ) == "=" )
					lnCurValue := Val( SubStr( lsCurrentParameter, 10 ) )
					p_sOutput := AllTrim( lnCurValue )
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_OUTPUT )
			EndIf
		Case lsCurrentParameter == "-O"
			++ p_nOptimise
			If ( Len( lsCurrentParameter) > 2 )
				p_nOptimise := Val(AllTrim( SubStr( lsCurrentParameter, 3 )))
			EndIf
		Case SubStr( lsCurrentParameter, 1, 10) == "--optimise"
			++ p_nOptimise
			If ( Len( lsCurrentParameter) > 10 )
				If ( SubStr( lsCurrentParameter, 11, 1 ) == "=" )
					lnCurValue := Val( SubStr( lsCurrentParameter, 10 ) )
					p_nOptimise := Val( AllTrim( lnCurValue ))
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_OPTIMISE )
			EndIf
		Case lsCurrentParameter == "-p"
			p_bpCode := .T.
		Case lsCurrentParameter == "--pcode"
			p_bpCode := .T.
		Case lsCurrentParameter == "-P"
			p_bPreprocess := .T.
		Case lsCurrentParameter == "--preprocess"
			p_bPreprocess := .T.
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-q"
			If ( Len( lsCurrentParameter) > 2 )
				lsCurrentParameter := SubStr( lsCurrentParameter, 3 )
			Else
				lsCurrentParameter := paParameters[ ++ln01ii ]
			EndIf
			if lsCurrentParameter == "as-word"
				p_bAsWord := .T.
			ElseIf lsCurrentParameter == "no-as-word"
				p_bAsWord := .F.
			Else
				aAdd( p_aDevilError, PARAM_ERROR_Q )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 8) == "--compat"
			If ( Len( lsCurrentParameter) > 8 )
				lsCurrentParameter := SubStr( lsCurrentParameter, 9 )
				If lsCurrentParameter == "as-word"
					p_bAsWord := .T.
				ElseIf lsCurrentParameter == "no-as-word"
					p_bAsWord := .F.
				Else
					aAdd( p_aDevilError, PARAM_ERROR_COMPAT )
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_COMPAT )
			EndIf
		Case lsCurrentParameter == "-r"
			p_bShared := .T.
		Case lsCurrentParameter == "--shared-exec"
			p_bShared := .T.
		Case lsCurrentParameter == "-R"
			p_bNoRemoveC := .T.
		Case lsCurrentParameter == "--noremove-c"
			p_bNoRemoveC := .T.
		Case lsCurrentParameter == "-s"
			p_bShared := .T.
		Case lsCurrentParameter == "--shared"
			p_bShared := .T.
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-S"
			If ( Len( lsCurrentParameter) > 2 )
				aAdd( p_sSourceCharset, SubStr( lsCurrentParameter, 3 ))
			Else
				aAdd( p_sSourceCharset, paParameters[ ++ln01ii ] )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 12) == "--source-charset"
			If ( Len( lsCurrentParameter) > 12 )
				If ( SubStr( lsCurrentParameter, 13, 1 ) == "=" )
					aAdd( p_sSourceCharset, SubStr( lsCurrentParameter, 14 ))
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_SOURCE_CHARSET )
			EndIf
		Case lsCurrentParameter == "-t"
			p_bShowSyntaxTree := .T.
		Case SubStr( lsCurrentParameter, 1, 8) == "--show-syntax-tree"
			p_bShowSyntaxTree := .T.
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-U"
			If ( Len( lsCurrentParameter) > 2 )
				aAdd( p_aUseStdCh, SubStr( lsCurrentParameter, 3 ))
			Else
				aAdd( p_aUseStdCh, paParameters[ ++ln01ii ] )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 12) == "--use-std-ch"
			If ( Len( lsCurrentParameter) > 12 )
				If ( SubStr( lsCurrentParameter, 13, 1 ) == "=" )
					aAdd( p_aUseStdCh, SubStr( lsCurrentParameter, 14 ))
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_USE_STD_CH )
			EndIf
		Case lsCurrentParameter == "-V"
			? "Devil version :", p_sDevilVersion
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-v"
			++ p_nDevilVerbosity
			If ( Len( lsCurrentParameter) > 3 )
				lnCurValue := Val( SubStr( lsCurrentParameter, 3 ) )
				If ( lnCurValue > 6 ) .or. ( lnCurValue < 0)
					aAdd( p_aDevilError, PARAM_ERROR_V )
					p_bStop := .T.
				Else
					p_nDevilVerbosity := lnCurValue
				EndIf
			EndIf
		Case SubStr( lsCurrentParameter, 1, 9 ) == "--verbose"
			++ p_nDevilVerbosity
			If ( Len( lsCurrentParameter) > 9 )
				If ( SubStr( lsCurrentParameter, 10, 1 ) == "=" )
					lnCurValue := Val( SubStr( lsCurrentParameter, 11 ) )
					If ( lnCurValue > 6 ) .or. ( lnCurValue < 0)
						aAdd( p_aDevilError, PARAM_ERROR_VERBOSE )
						p_bStop := .T.
					Else
						p_nDevilVerbosity := lnCurValue
					EndIf
				EndIf
			EndIf
		Case lsCurrentParameter == "-w"
			p_bNameSpaceWaring := .T.
		Case lsCurrentParameter == "--namespace-warning"
			p_bNameSpaceWaring := .T.
			If ( SubStr( lsCurrentParameter, 13, 1 ) == "=" )
				lnCurValue := Val( SubStr( lsCurrentParameter, 14 ) )
				If ( lnCurValue > 1 ) .or. ( lnCurValue < 0)
					aAdd( p_aDevilError, PARAM_ERROR_NAMESPACE )
					p_bStop := .T.
				Else
					p_bNameSpaceWaring := iIf( lnCurValue == 1, .T., .F. )
				EndIf
			EndIf
		Case SubStr( lsCurrentParameter, 1, 2 ) == "-T"
			If ( Len( lsCurrentParameter) > 2 )
				aAdd( p_sTargetCharset, SubStr( lsCurrentParameter, 3 ))
			Else
				aAdd( p_sTargetCharset, paParameters[ ++ln01ii ] )
			EndIf
		Case SubStr( lsCurrentParameter, 1, 12) == "--target-charset"
			If ( Len( lsCurrentParameter) > 12 )
				If ( SubStr( lsCurrentParameter, 13, 1 ) == "=" )
					aAdd( p_sTargetCharset, SubStr( lsCurrentParameter, 14 ))
				EndIf
			Else
				aAdd( p_aDevilError, PARAM_ERROR_TARGET_CHARSET )
			EndIf
		Case lsCurrentParameter == "--static"
			p_bFullStatic := .T.
		Case lsCurrentParameter == "--full-static"
			p_bFullStatic := .T.
		Case "=" $ lsCurrentParameter
			ln02ii := At ( "=", lsCurrentParameter )
			lnCurValue := SubStr ( lsCurrentParameter, 1, ln02ii - 1 )
			lnNewValue := SubStr ( lsCurrentParameter, ln02ii + 1 )
			&lnCurValue := lnNewValue
		Case ".PRG" == rAt( Upper( lsCurrentParameter), 4)
			p_sSourcePrg := lsCurrentParameter
		Case ".O" == rAt( UPPER( lsCurrentParameter), 2)
			aAdd( p_aObjs, lsCurrentParameter)
		Case ".SO" == rAt( UPPER( lsCurrentParameter), 3)
			aAdd( p_aLibrary, lsCurrentParameter)
		Case ".LIB" == rAt( UPPER( lsCurrentParameter), 4)
			aAdd( p_aLibrary, lsCurrentParameter)
		Case ".DLL" == rAt( UPPER( lsCurrentParameter), 4)
			aAdd( p_aLibrary, lsCurrentParameter)
		Case ".A" == rAt( UPPER( lsCurrentParameter), 2)
			aAdd( p_aLibrary, lsCurrentParameter)
		Otherwise
			p_bStop := .F.
				aAdd( p_aDevilError, lsCurrentParameter + PARAM_ERROR_INVALID )
		EndCase
		++ln01ii
	EndDo

