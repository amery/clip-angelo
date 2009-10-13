#include <fwin/CI_Objects.ch>


#ifndef CI__REDEFCLS_CH
#define CI__REDEFCLS_CH

// La funccion de sobrecarga de metodos de una clase debe llamarse
// antes de que dicha clase est� definida.
// La variable nClassH debe LLAMARSE ASI OBLIGATORIAMENTE. Esto es necesario
// para el buen funcionamiento de los comandos DATA, METHOD, ....

#xcommand function <cFunc> REDEFINE CLASS <cClassName> [ FROM <SupCls> ] => ;
    function _AsFunc_( <cFunc> )    ;;
	local nClassH               ;;
	if _ClassHandle( _AsStr_( <cClassName> ) ) == 0         ;;
	    _AsFunc_( <cClassName> )                            ;;
	    nClassH = _ClassHandle( _AsStr_( <cClassName> ) )   ;
	    [ ; #define _sUPcLS_ _AsName_( <SupCls> ) ]



#xcommand END REDEFINE CLASS => ;
	end                     ;;
	return nil              ;;
	#include <fwin/CI_obendcls.ch>


#endif
