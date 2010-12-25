STATIC FUNCTION NoExt( cFile )
    local i := at( ".", cFile )
RETURN alltrim( if( i > 0, left( cFile, i-1 ), cFile ) )
