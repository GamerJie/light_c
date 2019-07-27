
#include "log.h"
#include "system.h"
#include "binding.h"

#include <flecs.h>
#include <string.h>

const char *path = "scripts/systems/";

int lua_column(lua_State* L) {
	ecs_rows_t* rows = (ecs_rows_t*)lua_touserdata(L, 1);
	int type = lua_tonumber(L, 2);
	int column = lua_tonumber(L, 3);

	void* ret = _ecs_column(rows, type, column);
	lua_pushlightuserdata(L, ret);
	lua_pushnumber(L, rows->count);

	return 2;
}


void process(ecs_rows_t *rows) {
	System* system = (System*)rows->param;
	System_update(system, rows);
}


int System_init(System *self, char *file_name) {
	//m_system = self;
	self->name = file_name;
    self->lua = luaL_newstate();
    luaL_openlibs(self->lua);

	// binding ecs api function to lua
	lua_pushstring(self->lua, self->name);
	lua_setglobal(self->lua, "name");

	lua_pushcfunction(self->lua, lua_print);
	lua_setglobal(self->lua, "print");

	lua_pushcfunction(self->lua, lua_column);
	lua_setglobal(self->lua, "column");

	lua_pushcfunction(self->lua, lua_new_entity);
	lua_setglobal(self->lua, "entity");

	lua_pushcfunction(self->lua, lua_new_component);
	lua_setglobal(self->lua, "component");

	lua_pushcfunction(self->lua, lua_new_system);
	lua_setglobal(self->lua, "system");

	lua_pushcfunction(self->lua, lua_add_component);
	lua_setglobal(self->lua, "add_component");

	lua_pushcfunction(self->lua, lua_has_component);
	lua_setglobal(self->lua, "has_component");

	lua_pushcfunction(self->lua, lua_get_component);
	lua_setglobal(self->lua, "get_component");

	lua_pushcfunction(self->lua, lua_remove_component);
	lua_setglobal(self->lua, "remove_component");

	// get lua file name and dofile
    char *name = (char *) malloc(strlen(path) + strlen(file_name));
	assert(name != 0);
    sprintf(name, "%s%s.lua", path, file_name);

    luaL_dofile(self->lua, name);
    lua_getglobal(self->lua, "init");
    int ret = lua_pcall(self->lua, 0, 0, 0);

	// check lua file error
    if (ret) {
		log_error("Lua Error: [%s] [init] %s\n", file_name, lua_tostring(self->lua, -1));
        self->status = SYS_ERROR;
    } else {
        self->status = SYS_RUNNING;
    }

    self->process = process;

    return ret;
}

int System_update(System* self, struct ecs_rows_t *rows) {
    if(self->status != SYS_RUNNING)
        return self->status;

    lua_getglobal(self->lua, "update");
	lua_pushlightuserdata(self->lua, rows);
    int ret = lua_pcall(self->lua, 1, 0, 0);
    if(ret) {
        log_error("Lua Error: [update] %s\n", lua_tostring(self->lua, -1));
        self->status = SYS_ERROR;
    }


    return 0;
}

void System_clean(System* self) {
    lua_close(self->lua);
}