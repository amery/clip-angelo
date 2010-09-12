static function te_moveBlock(undo)
    undo := iif(undo==NIL, .t., undo)

RETURN ::copyBlock(.t., undo)
