#ifndef LIGHT_APP_H
#define LIGHT_APP_H

#include "occ.h"

CLASS(App,
	int app_running;

	unsigned int id;
    struct SDL_Window* window;
    struct SDL_Renderer* render;

	struct lua_State* lua;
	struct ecs_world * world;
)

int App_init(App* self, const char* title, int widht, int height);
int App_run(App* self);
void App_clean(App* self);

#endif