#ifndef TASKMANAGER
#define TASKMANAGER

#include <types.h>
#include <res/globals.h>
#include <linkedlistnode.h>
#include <microtask.h>

struct TaskManager;

typedef struct TaskManager {
	LinkedListNode* registeredTasks;
} TaskManager;

void TaskManager_ctor( TaskManager* this );
void TaskManager_dtor( TaskManager* this );

void TaskManager_addTask( TaskManager* this, Microtask* task );
void TaskManager_removeTask( TaskManager* this, Microtask* task );
void TaskManager_removeTaskByTag( TaskManager* this, Microtask_Tag tag );

void TaskManager_cycle( TaskManager* this );

#endif
