#ifndef LINKED_LIST
#define LINKED_LIST

#include <res/globals.h>
#include <types.h>

struct LinkedListNode;

typedef struct LinkedListNode {
	void* data;
	struct LinkedListNode* next;
} LinkedListNode;

typedef bool ( *LinkedListNode_SearchPredicate )( void* );
typedef void ( *LinkedListNode_IteratorFunction )( void* );

void LinkedListNode_ctor( LinkedListNode* this );
void LinkedListNode_dtor( LinkedListNode* this );

/**
 * Inserts at the end of the linked list (last node pointing to NULL)
 *
 * @param		{void*}		data		The data to be inserted into the linked list. Your container
 * 										method should keep track of the type of this data.
 */
void LinkedListNode_insertEnd( LinkedListNode* this, void* data );

/**
 * Find and return data in the list based on given criteria. Data will be returned from the first node
 * in which predicateFunction returns TRUE.
 *
 * @param		{LinkedListNode_SearchPredicate}		predicateFunction		This function shall return TRUE to signify the data has been found.
 *
 * @returns		{void*|NULL}													void* if data is found, NULL if data is not found.
 */
void* LinkedListNode_findData( LinkedListNode* this, LinkedListNode_SearchPredicate predicateFunction );

/**
 * Remove data in the linked list based on given criteria. The node causing predicateFunction to return TRUE
 * will be removed from the list. If removeAll is TRUE, the remove function will continue iterating through
 * the linked list until the final node is reached.
 *
 * NB: FIRST ARGUMENT IS THE ADDRESS OF A POINTER!
 *
 * @param		{LinkedListNode_SearchPredicate}		predicateFunction		This function shall return TRUE to signify the data has been found.
 * @param		{bool}									removeAll				If TRUE, continue to remove after the first removal.
 */
void LinkedListNode_remove( LinkedListNode** this, LinkedListNode_SearchPredicate predicateFunction, bool removeAll );

/**
 * Call iteratorFunction on each instance of data in this linked list.
 *
 * @param		{LinkedListNode_IteratorFunction}		iteratorFunction		This function will be called with each "data" pointer as an argument.
 */
void LinkedListNode_each( LinkedListNode* this, LinkedListNode_IteratorFunction iteratorFunction );

#endif
