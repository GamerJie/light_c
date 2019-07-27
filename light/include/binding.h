#ifndef LIGHT_BINDING_H
#define LIGHT_BINDING_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int lua_print(lua_State* L);

int lua_new_entity(lua_State* L);

int lua_new_component(lua_State* L);

int lua_new_system(lua_State* L);

int lua_add_component(lua_State* L);

int lua_has_component(lua_State* L);

int lua_remove_component(lua_State* L);

int lua_get_component(lua_State* L);

#endif