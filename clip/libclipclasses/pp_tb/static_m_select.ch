static func m_select(self, npos)
   if npos>=0 .and. npos<=self:itemCount
      self:current:=npos
   endif
return self
