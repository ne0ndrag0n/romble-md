#ifndef SUPERVISOR_STATES
#define SUPERVISOR_STATES

// Had to be in a separate file because of circular dependencies

typedef enum {
	Supervisor_State_DEFAULT = 0,
	Supervisor_State_WELCOME,
	Supervisor_State_LOGIN,
	Supervisor_State_BETATEST
} Supervisor_State;

#endif
