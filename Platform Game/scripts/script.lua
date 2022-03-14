function coro(entity)
	while true do
		CMove(entity, 0.5, 0.3); coroutine.yield()
		CMove(entity, -0.5, 0.5); coroutine.yield()
		CMove(entity, -0.5, -0.6); coroutine.yield()
		CMove(entity, 0.2, -0.3); coroutine.yield()
	end
end

local c = coroutine.create(coro)

function run(entity)
	 if coroutine.status(c) ~= 'dead' then
		coroutine.resume(c, entity)
	 end
end

