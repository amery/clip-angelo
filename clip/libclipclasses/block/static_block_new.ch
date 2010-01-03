static function block_new(self, cExpression )

   if Type( cExpression ) $ "UEUI"
      MsgAlert( [Class TBlock constructor error:Invalid expression] )
   else
      self:cExpression = cExpression
      self:bBlock      = &( cExpression )
   endif

return self
