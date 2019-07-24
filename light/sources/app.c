
#include <SDL.h>

#include <flecs.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "app.h"
#include "debug.h"
#include "system.h"

App* app;

int lua_new_entity(lua_State* L) {
    const char* compontens = luaL_checkstring(L, 1);

	const char* base_name = "lua_entity";
	char* name = (char*)malloc(strlen(base_name) + 1);
	sprintf(name, "%s%d.lua", base_name, app->id++);

    ecs_entity_t ent = ecs_new_entity(app->world, name, compontens);
    ecs_type_t Tent = ecs_type_from_entity(app->world, ent);
    (void)ent;
    (void)Tent;

    lua_pushinteger(L, ent);

    return 1;
}

int lua_new_component(lua_State* L) {
	int size = luaL_checkinteger(L, 1);
	const char* name = luaL_checkstring(L, 2);
	ecs_entity_t ecs_entity(pos) = ecs_new_component(app->world, name, size);
	ecs_type_t Tpos = ecs_type_from_entity(app->world, ecs_entity(pos));
	(void)ecs_entity(pos);
	(void)Tpos;

	return 0;
}

int lua_new_sys(lua_State* L) {
    const char* id = luaL_checkstring(L, 1);
    int type = luaL_checkint(L, 2);
    const char* types = luaL_checkstring(L, 3);
//    printf("new system: %s  %d  %s\n", id, type, types);

    System sys;
    System_init(&sys, id);

    ecs_entity_t f_system = ecs_new_system(app->world, id, type, types, sys.process);
    ecs_entity_t system = f_system;
    ecs_type_t t_system = ecs_type_from_entity(app->world, system);
    (void)system;
    (void)t_system;

    return 0;
}

int lua_add_component(lua_State* L) {
	int ent = luaL_checkinteger(L, 1);
	const char* type = luaL_checkstring(L, 2);

	_ecs_add(app->world, ent, type);

	return 0;
}

int lua_has_component(lua_State* L) {
	int ent = luaL_checkinteger(L, 1);
	const char* type = luaL_checkstring(L, 2);

	_ecs_has(app->world, ent, type);

	return 0;
}


int App_init(App* self, const char* title, int width, int height) {
	// app singleton mode
    ERR(app, "%s", "only can init one app !");
    app = self;
	self->id = 1;

	// init sdl enviourment
	int ret = SDL_Init(SDL_INIT_EVERYTHING);
	ERR(!ret, "%s", SDL_GetError());

	self->window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_SHOWN);
	ERR(!self->window, "%s", SDL_GetError());

	self->render = SDL_CreateRenderer(self->window, -1, 0);
	ERR(!self->render, "%s", SDL_GetError());

	self->app_running = 1;

    self->lua = luaL_newstate();
    luaL_openlibs(self->lua);

	// binding world api function to lua
	lua_pushcfunction(self->lua, lua_new_entity);
	lua_setglobal(self->lua, "entity");

	lua_pushcfunction(self->lua, lua_new_component);
	lua_setglobal(self->lua, "component");

    lua_pushcfunction(self->lua, lua_new_sys);
    lua_setglobal(self->lua, "system");

	lua_pushcfunction(self->lua, lua_add_component);
	lua_setglobal(self->lua, "add_component");

	// init default world
	self->world = ecs_init();

	// lua main logic entry
    ret = luaL_dofile(self->lua, "scripts/main.lua");
    if(ret) {
		printf("Lua Error: [init] %s\n", lua_tostring(self->lua, -1));
		return ret;
	}

	return 0;
error:
	return -1;
}


int App_run(App* self) {
	SDL_Event _event;
	while (self->app_running) {
		if (SDL_PollEvent(&_event)) {
			if (_event.type == SDL_QUIT)
				break;
		}

		// todo feature : multi-world support 
		ecs_progress(self->world, 0);

		SDL_RenderClear(self->render);

		SDL_RenderPresent(self->render);
	}

	return 0;
}

void App_clean(App* self) {
	if (self->render)
		SDL_DestroyRenderer(self->render);
	if (self->window)
		SDL_DestroyWindow(self->window);

	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}