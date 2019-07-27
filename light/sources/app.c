
#include <SDL.h>
#include <flecs.h>

#include "app.h"
#include "log.h"
#include "debug.h"
#include "system.h"
#include "binding.h"
#include "render.h"

App* app;

int lua_print(lua_State* L) {
	int args = lua_gettop(L);

	char *msg = malloc(2048);
	char* temp = malloc(1024);
	sprintf(msg, "%s", "");

	for (int i = 1; i <= args; i++) {

		int type = lua_type(L, i);
		switch (type)
		{
		case LUA_TNUMBER:
			sprintf(temp, "%g\t", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			sprintf(temp, "%s\t", lua_tostring(L, i));
			break;
		case LUA_TTABLE:
			sprintf(temp, "%s", "[table]\t");
			break;
		case LUA_TNIL:
			sprintf(temp, "%s", "[nil]\t");
			break;
		case LUA_TUSERDATA:
			sprintf(temp, "%s", "[userdate]\t");
			break;
		case LUA_TBOOLEAN:
			sprintf(temp, "%s\t", lua_toboolean(L, i) ? "[true]" : "[false]");
			break;
		case LUA_TFUNCTION:
			sprintf(temp, "%s", "[function]\t");
			break;
		case LUA_TTHREAD:
			sprintf(temp, "%s", "[thread]\t");
			break;
		case LUA_TLIGHTUSERDATA:
			sprintf(temp, "[l_userdata]\t");
			break;
		default:
			break;
		}

		sprintf(msg, "%s%s", msg, temp);
	}

	lua_getglobal(L, "name");
	const char* name = luaL_checkstring(L, -1);

	lua_Debug ar;

	lua_getstack(L, 1, &ar);
	lua_getinfo(L, "l", &ar);

	log_log(LOG_INFO, name, ar.currentline , msg);

	return 0;
}

int lua_new_entity(lua_State* L) {
    const char* compontens = luaL_checkstring(L, 1);

	const char* base_name = "lua_entity";
	char temp[10];
	char* name = (char*)malloc(strlen(base_name) + strlen(itoa(app->id, temp, 10)));
	assert(name != NULL);
	sprintf(name, "%s%d.lua", base_name, app->id++);

    ecs_entity_t e_ent = ecs_new_entity(app->world, name, compontens);
    ecs_type_t t_ent = ecs_type_from_entity(app->world, e_ent);

    lua_pushinteger(L, e_ent);
	lua_pushlightuserdata(L, t_ent);

    return 2;
}

int lua_new_component(lua_State* L) {
	int size = luaL_checkinteger(L, 1);
	const char* com = luaL_checkstring(L, 2);
	ecs_entity_t e_com = ecs_new_component(app->world, com, size);
	ecs_type_t t_com = ecs_type_from_entity(app->world, e_com);

	lua_pushinteger(L, e_com);
	lua_pushlightuserdata(L, t_com);

	return 2;
}

int lua_new_system(lua_State* L) {
    const char* id = luaL_checkstring(L, 1);
    int type = luaL_checkint(L, 2);
    const char* types = luaL_checkstring(L, 3);

    System * sys = malloc(sizeof(System));
    System_init(sys, id);
	assert(sys);
    ecs_entity_t e_system = ecs_new_system(app->world, id, type, types, sys->process);
    ecs_type_t t_system = ecs_type_from_entity(app->world, e_system);

	ecs_set_system_context(app->world, e_system, sys);

	lua_pushinteger(L, e_system);
	lua_pushlightuserdata(L, t_system);

    return 2;
}

int lua_add_component(lua_State* L) {
	int ent = luaL_checkint(L, 1);
	ecs_type_t type = lua_touserdata(L, 2);

	_ecs_add(app->world, ent, type);

	return 0;
}

int lua_has_component(lua_State* L) {
	int ent = luaL_checkint(L, 1);
	ecs_type_t type = lua_touserdata(L, 2);

	bool ret = _ecs_has(app->world, ent, type);

	lua_pushboolean(app->lua, ret);

	return 1;
}

int lua_remove_component(lua_State* L) {
	int ent = luaL_checkint(L, 1);
	ecs_type_t type = lua_touserdata(L, 2);

	_ecs_remove(app->world, ent, type);

	return 0;
}

int lua_get_component(lua_State* L) {
	int ent = luaL_checkint(L, 1);
	ecs_type_t type = lua_touserdata(L, 2);

	void* data = _ecs_get_ptr(app->world, ent, type);

	lua_pushlightuserdata(L, data);

	return 1;
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
	lua_pushstring(self->lua, "main.lua");
	lua_setglobal(self->lua, "name");

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

	lua_pushcfunction(self->lua, lua_print);
	lua_setglobal(self->lua, "print");

	// init default world
	self->world = ecs_init();

	// lua main logic entry
    ret = luaL_dofile(self->lua, "scripts/main.lua");
    if(ret) {
		log_error("Lua Error: [init] %s\n", lua_tostring(self->lua, -1));
		return ret;
	}

	return 0;
error:
	return -1;
}

int App_run(App* self) {
	Render* render = malloc(sizeof(Render));
	Render_init(render, self);

	SDL_Event _event;
	while (self->app_running) {
		if (SDL_PollEvent(&_event)) {
			if (_event.type == SDL_QUIT)
				break;
		}

		SDL_RenderClear(self->render);

		// todo feature : multi-world support 
		ecs_progress(self->world, 0);

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