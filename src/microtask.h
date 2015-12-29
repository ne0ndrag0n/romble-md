#ifndef MICROTASK
#define MICROTASK

#include <types.h>
#include <res/globals.h>

struct Microtask;

typedef u8 Microtask_Tag;
typedef u16 Microtask_State;
typedef struct {
	void ( *destroy )( struct Microtask* );
	void ( *execute )( struct Microtask* );
} Microtask_vtable;

extern const Microtask_State Microtask_State_COMPLETE;
extern const Microtask_State Microtask_State_RUNNING;
extern const Microtask_State Microtask_State_PAUSED;

typedef struct Microtask {
	Microtask_Tag taskTag;
	Microtask_State currentState;

	vtable functions;
} Microtask;

void Microtask_ctor( Microtask* this );
void Microtask_dtor( Microtask* this );

void Microtask_execute( Microtask* this );

#endif
