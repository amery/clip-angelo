static func sbv_display(self)
   local i,ccc
   ccc:=self:current
   if ccc< (self:total/2)
      ccc--
   endif
   self:thumbPos:=round((ccc/self:total)*self:barLength,0)
   self:thumbPos:=min(self:barLength,max(1,self:thumbPos))
   dispOutAt(self:start,self:offset, substr(self:style,1,1), self:colorSpec)
   for i=self:start+1 to self:end-1
       dispOutAt(i, self:offset, substr(self:style,2,1), self:colorSpec)
   next
   dispOutAt(self:start+self:thumbPos, self:offset,;
           substr(self:style,3,1), self:colorSpec )
   dispOutAt(self:end, self:offset, substr(self:style,4,1), self:colorSpec)
   setpos(self:start+self:thumbPos, self:offset)
return self
