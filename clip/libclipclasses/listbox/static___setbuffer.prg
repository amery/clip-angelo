static function __setBuffer
    if ::__bufType==0
       ::buffer:=::value
    endif
    if ::__bufType==1
       ::buffer:=::__mtext[::value]
    endif
    if ::__bufType==2
       ::buffer:=::__mdata[::value]
    endif
return ::buffer
