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
	bool firstNodeFreed = FALSE;

	while( current != NULL ) {
		if( predicateFunction( current->data ) == TRUE ) {
			if( prev != NULL ) {
				prev->next = current->next;
				Romble_free_d( current, FILE_LINE() );
				current = prev->next;
			} else {
				firstNodeFreed = TRUE;
				LinkedListNode* relink = current->next;
				Romble_free_d( current, FILE_LINE() );
				current = relink;

				// We need to redefine where this actually points to, since it no longer exists
				*this = current;
			}

			if( removeAll != TRUE ) {
				if( firstNodeFreed == TRUE && prev == NULL ) {
					*this = NULL;
				}
				return;
			}
		} else {
			prev = current;
			current = current->next;
		}
	}

	// If we never encountered a second node (prev remains NULL) yet a free operation took place,
	// we NEED to set *this to NULL to prevent the porential access of freed RAM.
	if( firstNodeFreed == TRUE && prev == NULL ) {
		*this = NULL;
	}
}
