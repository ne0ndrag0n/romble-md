/**
 * Bitwise Trie cfile
 *
 * @author		Ash
 * @version		0.0.1
 */

#include <bit_trie.h>
#include <types.h>
#include <stddef.h>
#include <stdlib.h>
#include <res/globals.h>
#include <romble.h>
#include <utility.h>
#include <lang.h>

void BitwiseTrieNode_ctor( BitwiseTrieNode* this, bool leaf ) {
	if( leaf == TRUE ) {
		this->data = NULL;
	} else {
		// Allocate four child elements; they will be zeroed out
		this->children = Romble_alloc_d( sizeof( BitwiseTrieNode* ) * 4, TRUE, FILE_LINE() );
		Romble_assert( this->children != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
	}
}

void BitwiseTrieNode_dtor( BitwiseTrieNode* this ) {
	// Recursively remove all elements
}

void BitwiseTrieNode_insert( BitwiseTrieNode* this, u8 key, void* value ) {
	BitwiseTrieNode* current = this;

	u8 index;
	size_t i;
	for( i = 0; i != 3; i++ ) {
		index = ( key >> ( i * 2 ) ) & 0x03;

		if( current->children[ index ] == NULL ) {
			// Need to create a new BitwiseTrieNode
			current->children[ index ] = Romble_alloc_d( sizeof( BitwiseTrieNode ), TRUE, FILE_LINE() );
			Romble_assert( current->children[ index ] != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
			// Now initialize the four items inside
			BitwiseTrieNode_ctor( current->children[ index ], FALSE );
		}

		current = current->children[ index ];
	}

	// Get upper nibble of 8-bit key
	index = ( key >> 6 ) & 0x03;

	// Create a new BitwiseTrieNode and initialise it to a leaf if it does not exist
	// If it DOES exist, we will simply overwrite it
	if( current->children[ index ] == NULL ) {
		current->children[ index ] = Romble_alloc_d( sizeof( BitwiseTrieNode ), TRUE, FILE_LINE() );
		Romble_assert( current->children[ index ] != NULL, FILE_LINE( EXCEPTION_OUT_OF_MEMORY ) );
		BitwiseTrieNode_ctor( current->children[ index ], TRUE );
	}

	// One last current
	current = current->children[ index ];

	// Set the value
	current->data = value;
}

void* BitwiseTrieNode_get( BitwiseTrieNode* this, u8 key ) {
	BitwiseTrieNode* current = this;

	u8 index;
	size_t i;
	// Every step of the way, we're going to assume this entire tree exists down to the leaf.
	// If it does not, we will return NULL as the result was not found
	for( i = 0; i != 4; i++ ) {
		index = ( key >> ( i * 2 ) ) & 0x03;

		if( current->children[ index ] == NULL ) {
			return NULL;
		}

		current = current->children[ index ];
	}

	return current->data;
}

/**
 * Proxies to recursive key helper call
 */
void BitwiseTrieNode_delete( BitwiseTrieNode* this, u8 key ) {
	BitwiseTrieNode_deleteHelper( this, NULL, key, 0 );
}

void BitwiseTrieNode_deleteHelper( BitwiseTrieNode* current, BitwiseTrieNode* parent, u8 key, u8 step ) {

	// Stop everything if we are null!
	if( current == NULL ) {
		return;
	}

	if( step == 4 ) {

		free( current );
		// Null out the parent reference
		parent->children[ BitwiseTrieNode_getCell( key, step - 1 ) ] = NULL;

	} else {

		u8 indexOfNext = BitwiseTrieNode_getCell( key, step );
		BitwiseTrieNode* nextChild = current->children[ indexOfNext ];

		BitwiseTrieNode_deleteHelper( nextChild, current, key, step + 1 );

		// Afterward, free this node and change parent reference to NULL if all are zero
		// Check if all of current's children is null on the way back up
		if( parent != NULL ) {
			u8 index;

			// Bail out early if there's any non-null child
			for( index = 0; index != 4; index++ ) {
				if( current->children[ index ] != NULL ) {
					return;
				}
			}

			// Free current
			free( current );
			// Null out parent's reference to current
			parent->children[ BitwiseTrieNode_getCell( key, step - 1 ) ] = NULL;
		}

	}
}

inline u8 BitwiseTrieNode_getCell( u8 key, u8 step ) {
	return ( key >> ( step * 2 ) ) & BitwiseTrieNode_CELL_MASK;
}
