static function run_ontick()
	while on_tick_enable
		eval(on_tick_block)
		sleep(on_tick_period)
	end
return
