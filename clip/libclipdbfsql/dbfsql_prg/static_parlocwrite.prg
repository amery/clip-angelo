STATIC PROCEDURE ParLocWrite(conn,pars,RDBMS)
	LOCAL i
	IF VALTYPE(pars) == "A"
		FOR i:=1 to LEN(pars)
			IF VALTYPE(pars[i][2]) == "C" .AND.;
				(LEN(pars[i]) < 4 .OR. VALTYPE(pars[i][4]) != "L" .OR. !pars[i][4])
				SQLLocWrite(conn,@pars[i][2])
			ENDIF
			IF RDBMS == "MS" .OR. RDBMS == "PG" .OR. RDBMS == "ODBC";
				.OR. RDBMS == "DBTCP"

				IF VALTYPE(pars[i][2]) == "C"
					IF !(LEFT(pars[i][2],1) == "'" .AND. RIGHT(pars[i][2],1) == "'")
						pars[i][2] := "'"+pars[i][2]+"'"
					ENDIF
				ELSEIF VALTYPE(pars[i][2]) == "N"
					pars[i][2] := ALLTRIM(STR(pars[i][2]))
				ELSEIF VALTYPE(pars[i][2]) == "D"
					pars[i][2] := "'"+STRZERO(YEAR(pars[i][2]),4)+"-"+;
						STRZERO(MONTH(pars[i][2]),2)+"-"+;
						STRZERO(DAY(pars[i][2]),2)+"'"
				ENDIF
			ENDIF
		NEXT
	ENDIF
RETURN
