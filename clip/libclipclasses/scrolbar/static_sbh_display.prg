static func sbh_display(self)
   local i,ccc
   ccc:=self:current
   if ccc< (self:total/2)
      ccc--
   endif
   self:thumbPos:=round((ccc/self:total)*self:barLength,0)
   self:thumbPos:=min(self:barLength,max(1,self:thumbPos))
   dispOutAt(self:offset,self:start, substr(self:style,1,1), self:colorSpec)
   for i=self:start+1 to self:end-1
       dispOutAt(self:offset, i, substr(self:style,2,1), self:colorSpec)
   next
   dispOutAt(self:offset, self:start+self:thumbPos,;
            substr(self:style,3,1), self:colorSpec )
   dispOutAt(self:offset, self:end, substr(self:style,4,1), self:colorSpec)
   setpos(self:offset, self:start+self:thumbPos)
return self
