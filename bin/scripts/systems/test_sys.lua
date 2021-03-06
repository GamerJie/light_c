
local ffi = require "ffi"
ffi.cdef [[
    typedef struct pos {
        int x;
        int y;
    } pos;
	
	typedef struct speed {
		float value;
	} speed;
]]

function sizeof(type)
	return ffi.sizeof(ffi.new(type))
end


function init()
	print("init system")
end

function update(rows)
	local poss, count = column(rows, sizeof("pos"), 1)
	local speeds = column(rows, sizeof("speed"), 2)

	for i = 0, count - 1 do
		local pos = ffi.cast("pos*", poss)[i]
		local speed = ffi.cast("speed*", speeds)[i]

		-- print(pos.x, pos.y, speed.value, count)
	end
	-- print("==============")
end