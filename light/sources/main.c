
#include "app.h"

#define DEBUG 1

#ifdef WIN32
#ifdef DEBUG
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
#endif
#endif // WIN32

int main(int argc, char* argv[]) {
	App app;
	int ret = App_init(&app, "ECS Demo", 800, 600);
	App_run(&app);

	App_clean(&app);

    return 0;
}