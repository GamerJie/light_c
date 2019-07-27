#ifndef LIGHT_RENDER_H
#define LIGHT_RENDER_H

#include "app.h"

typedef struct sprite {
	int loaded;
	char* file_path;
	void* texture;
} sprite;

CLASS(Render,
	App* app;
)

void Render_init(Render *self, App* app);


#endif