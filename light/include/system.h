//
// Created by Jie on 19.7.22.
//

#ifndef LIGHT_SYSTEM_H
#define LIGHT_SYSTEM_H

#include "occ.h"

typedef enum {
    SYS_RUNNING,
    SYS_PAUSE,
    SYS_ERROR
} SYS_STATE;

CLASS(System,
    SYS_STATE status;
    struct lua_State* lua;

    void (*process)(struct ecs_rows_t* rows);
)

int System_init(System* self, const char* file_name);
int System_update(System* self, struct ecs_rows_t* rows);
void System_clean(System* self);

#endif //LIGHT_SYSTEM_H
