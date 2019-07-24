
local ffi = require "ffi"

ffi.cdef [[
    typedef struct pos {
        int x;
        int y;
    } pos;

	typedef struct speed {
		int x;
		int y;
	} speed;

    int size() {

    };
]]


local p = ffi.new("pos", {x = 40, y = 2})
local s = ffi.string(ffi.cast("char*", p))
-- test(s)

print(ffi.sizeof(p))

component(ffi.sizeof(ffi.new("pos")), "pos")
component(ffi.sizeof(ffi.new("speed")), "speed")

system("test_sys", 3, "speed")
-- new_system("test_sys", 3, "speed")

local ent1 = entity("pos")

-- add_component(ent1, "speed")

