static function ErrorMessage(e)
local cMessage
// start error message
cMessage := if( e:severity > ES_WARNING, "Error ", "Warning " )
// add subsystem name if available
if ( ValType(e:subsystem) == "C" )
  cMessage += e:subsystem()
else
  cMessage += "???"
end
// add subsystem's error code if available
if ( ValType(e:subCode) == "N" )
  cMessage += ("/" + NTRIM(e:subCode))
else
  cMessage += "/???"
end
// add error description if available
if ( ValType(e:description) == "C" )
  cMessage += ("  " + e:description)
end
// add either filename or operation
if ( !Empty(e:filename) )
  cMessage += (": " + e:filename)
elseif ( !Empty(e:operation) )
  cMessage += (": " + e:operation)
end
return (cMessage)
