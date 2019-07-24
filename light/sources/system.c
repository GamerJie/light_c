//
// Created by Jie on 19.7.22.
//

#include "system.h"

#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <flecs.h>

System *m_system;
const char *path = "scripts/systems/";

void process(ecs_rows_t *rows) {
	//System_update(m_system, rows);
	//speed* p = ecs_column(rows, speed, 1);
	//speed* p = _ecs_column(rows, sizeof(speed), 1);

	/*lua_State* L = m_system->lua;
	lua_getglobal(L, "update");
	lua_pcall(L, 0, 0, -1);*/
	
	printf("pos count: %d\n", rows->count);
}


int System_init(System *self, const char *file_name) {
	m_system = self;
    self->lua = luaL_newstate();
    luaL_openlibs(self->lua);

    char *name = (char *) malloc(strlen(path) + strlen(file_name));
    sprintf(name, "%s%s.lua", path, file_name);

    luaL_dofile(self->lua, name);
    lua_getglobal(self->lua, "init");
    int ret = lua_pcall(self->lua, 0, 0, 0);

	
    if (ret) {
        printf("Lua Error: [%s] [init] %s\n", file_name, lua_tostring(self->lua, -1));
        self->status = SYS_ERROR;
    } else {
        self->status = SYS_RUNNING;
    }

    self->process = process;

    return ret;
}

int System_update(System* self, struct ecs_rows_t *rows) {
    if(self->status == SYS_ERROR)
        return self->status;

    lua_getglobal(self->lua, "update");
	lua_pushlightuserdata(self->lua, rows);
    int ret = lua_pcall(self->lua, 1, 0, 0);
    if(ret) {
        printf("Lua Error: [update] %s\n", lua_tostring(self->lua, -1));
        self->status = SYS_ERROR;
    }


    return ret;
}

void System_clean(System* self) {
    lua_close(self->lua);
}