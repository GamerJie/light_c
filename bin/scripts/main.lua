
local ffi = require "ffi"

ffi.cdef [[
    typedef struct pos {
        int x;
        int y;
    } pos;
	
	typedef struct speed {
		float value;
	} speed;

	typedef struct sprite {
		int loaded;
		char* file_path;
		void* texture;
	} sprite;
]]

function sizeof(type)
	return ffi.sizeof(ffi.new(type))
end


local p = ffi.new("pos", {x = 40, y = 2})
local s = ffi.string(ffi.cast("char*", p))
-- test(s)

print(ffi.sizeof(p))

local _, pos = component(sizeof("pos"), "pos")
local _, speed = component(sizeof("speed"), "speed")

system("test_sys", 3, "pos, speed")
system("move_sys", 3, "speed")

print("new system")

local ent1 = entity("pos")
local ent2 = entity("speed")

print("new entity")

add_component(ent1, speed)
-- add_component(ent2, speed)
print("add component")

if has_component(ent1, pos) then
	print("has component")
	local p_pos = get_component(ent1, pos)
	local c_pos = ffi.cast("pos*", p_pos)
	print("pos", c_pos.x, c_pos.y)
	c_pos.x = 13
	c_pos.y = 24
	
	print("set pos", c_pos.x, c_pos.y)
	-- set_component(ent1, pos, p_pos)
end

if has_component(ent1, speed) then
	local p_speed = get_component(ent2, speed)
	local c_speed = ffi.cast("speed*", p_speed)
	print("speed", c_speed.value)
	c_speed.value = 23.3

	print("set speed", c_speed.value);
	--set_component(ent2, speed, p_speed)
end

local _, sprite = component(sizeof("sprite"), "sprite")
add_component(ent1, sprite)

if has_component(ent1, sprite) then
	local p_sprite = get_component(ent1, sprite)
	local c_sprite = ffi.cast("sprite*", p_sprite)
	print("sprite", c_sprite.file_path)
	c_sprite.loaded = 0
	c_sprite.file_path = ffi.cast("char*", "images/main.bmp")

	print("set sprite", c_sprite.file_path);
end