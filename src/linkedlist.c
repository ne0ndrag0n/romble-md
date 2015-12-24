#include <linkedlist.h>
#include <romble.h>
#include <stdlib.h>
#include <lang.h>

void LinkedListNode_ctor( LinkedListNode* this ) {
	this->data = NULL;
	this->next = NULL;
}

void LinkedListNode_dtor( LinkedListNode* this ) {

	if( this == NULL ) {
		return;
	}

	if( this->next != NULL ) {
		LinkedListNode_dtor( this->next );
	}

	Romble_free_d( this, FILE_LINE() );
}

void LinkedListNode_insertEnd( LinkedListNode* this, void* data ) {
	LinkedListNode* current = this;

	while( current->next != NULL ) {
		current = current->next;
	}

	current->next = Romble_alloc_d( sizeof( LinkedListNode ), TRUE, FILE_LINE() );
	Romble_assert( current->next != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
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

void LinkedListNode_remove( LinkedListNode** this, LinkedListNode_SearchPredicate predicateFunction, bool removeAll ) {
	LinkedListNode* prev = NULL;
	LinkedListNode* current = *this;

	while( current != NULL ) {
		if( predicateFunction( current->data ) == TRUE ) {
			if( prev != NULL ) {
				prev->next = current->next;
				Romble_free_d( current, FILE_LINE() );
				current = prev->next;
			} else {
				LinkedListNode* relink = current->next;
				Romble_free_d( current, FILE_LINE() );
				current = relink;

				// We need to redefine where this actually points to, since it no longer exists
				// If this was the only node, current->next should've been NULL, which this
				// will now point to
				*this = current;
			}

			if( removeAll != TRUE ) {
				return;
			}
		} else {
			prev = current;
			current = current->next;
		}
	}
}
