#include <linkedlist.h>
#include <romble.h>
#include <stdlib.h>

void LinkedListNode_ctor( LinkedListNode* this ) {
	this->data = NULL;
	this->next = NULL;
}

void LinkedListNode_dtor( LinkedListNode* this ) {

	if( this->next != NULL ) {
		LinkedListNode_dtor( this->next );
	}

	free( this );
}

void LinkedListNode_insertEnd( LinkedListNode* this, void* data ) {
	LinkedListNode* current = this;

	while( current->next != NULL ) {
		current = current->next;
	}

	current->next = Romble_alloc_d( sizeof( LinkedListNode ), TRUE, FILE_LINE() );
	LinkedListNode_ctor( current->next );
	current = current->next;
	current->data = data;
}

void* LinkedListNode_findData( LinkedListNode* this, LinkedListNode_SearchPredicate predicateFunction ) {
	LinkedListNode* current = this;

	while( current != NULL ) {
		if( predicateFunction( current->data ) == TRUE ) {
			return current->data;
		}

		current = current->next;
	}

	// Nothing was found
	return NULL;
}
