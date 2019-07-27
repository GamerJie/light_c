
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
	local speeds, count = column(rows, sizeof("speed"), 1)

	for i = 0, count - 1 do
		local speed = ffi.cast("speed*", speeds)[i]

		-- print(speed.value, count)
	end
	-- print("==============")
end