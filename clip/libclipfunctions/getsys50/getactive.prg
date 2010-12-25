func GetActive(g)
local oldActive := status:ActiveGet
	if ( PCount() > 0 )
		status:ActiveGet := g
	end
return ( oldActive )
