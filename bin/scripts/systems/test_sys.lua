
local ffi = require "ffi"
ffi.cdef [[
    typedef struct pos {
        int x;
        int y;
    } pos;
]]

function init()
	print("init system")
end

function update(rows)
	print(rows)
end