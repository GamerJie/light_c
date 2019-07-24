#ifndef LIGHT_OCC_H
#define LIGHT_OCC_H

#include <stdio.h>
#include <stdlib.h>

#define CLASS(name, ...)	\
	typedef struct name {	\
		__VA_ARGS__			\
	} name;


#define CLASS_EXT(name, super, ...)	\
	typedef struct {				\
		union {						\
			struct super;			\
			super parent;			\
		};							\
		__VA_ARGS__					\
	} name;


#define INIT(type,object)               type##_init((type *) &object)

// sf stands for speed factor
#define UPDATE(object,sf)               (object).update(&object, sf); 
#define CLEAN(object)                   (object).clean(&object);

#define REGISTER(enroller,object_type,object) \
    (enroller).register##object_type((&enroller),(object_type *) &object)

#define FREE(object)            \
    if (object)                 \
        free(object);           \
    else                        \
        fprintf(stderr, "%s : %d :: Tried to free null ptr\n", __FILE__, __LINE__)

#define NOTIFY(notifier,src_ent,event_type) \
    ((notifier).notify(&notifier,src_ent,event_type))

#define IN_RANGE_INC(x,min,max) (x >  min && x <  max)
#define IN_RANGE_EXC(x,min,max) (x >= min && x <= max)
#endif