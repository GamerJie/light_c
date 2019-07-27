
#include "render.h"

#include <SDL.h>
#include <flecs.h>

void render_process(ecs_rows_t* rows) {
	Render* render = (Render*)rows->param;
	sprite* sprites = ecs_column(rows, sprite, 1);

	for (int i = 0; i < rows->count; i++) {
		// check file loaded
		if (!sprites[i].loaded) {
			SDL_Surface* img = SDL_LoadBMP(sprites[i].file_path);
			sprites[i].texture = SDL_CreateTextureFromSurface(render->app->render, img);

			sprites[i].loaded = 1;
		}

		assert(sprites[i].texture);
		// doto render the sprite
		SDL_RenderCopy(render->app->render, sprites[i].texture, NULL, NULL);
	}
}

void Render_init(Render* self, App* app) {
	self->app = app;

	// add component to world
	ecs_entity_t e_com = ecs_new_component(app->world, "sprite", sizeof(sprite));
	ecs_type_t t_com = ecs_type_from_entity(app->world, e_com);

	//ECS_SYSTEM(app->world, render_process, EcsOnUpdate, e_com);

	ecs_entity_t e_system = ecs_new_system(app->world, "render", EcsOnUpdate, "sprite", render_process);
	ecs_type_t t_system = ecs_type_from_entity(app->world, e_system);

	ecs_set_system_context(app->world, e_system, self);
}