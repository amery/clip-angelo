STATIC FUNCTION blobRead(self,oid_fd,buffer,length)
RETURN SQLBlobRead(self:conn,oid_fd,@buffer,length)
