func ErrorMessage(e)
local cMessage,i


	cMessage := if( e:severity > ES_WARNING, "Error ", "Warning " )

	if ( ValType(e:subsystem) == "C" )
		cMessage += e:subsystem
	else
		cMessage += "???"
	end


	if ( ValType(e:subCode) == "N" )
		cMessage += ("/" + NTRIM(e:subCode))
	else
		cMessage += "/???"
	end
	cmessage+=";"

	if ( ValType(e:description) == "C" )
		cMessage += e:description
	end

	if ( !Empty(e:operation) )
		cMessage += (": " + e:operation)
	end

	if ( !Empty(e:filename) )
		cMessage += (": " + e:filename)
	end

	if ( ValType(e:syserr) == "C" )
		cMessage += (";"+e:syserr)
	end

	outlog("error message",cMessage)
	i := 1
	while ( !Empty(ProcName(i)) )
		outlog("Called from", Trim(ProcName(i)) + ;
			"(" + NTRIM(ProcLine(i)) + ")  ")

		i++
	end
return (cMessage)
