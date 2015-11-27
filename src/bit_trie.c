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

void BitwiseTrieNode_ctor( BitwiseTrieNode* this, bool leaf ) {
	if( leaf == TRUE ) {
		this->data = NULL;
	} else {
		// Allocate four child elements; they will be zeroed out
		this->children = Romble_alloc_d( sizeof( BitwiseTrieNode* ) * 4, TRUE, FILE_LINE() );
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
	u8 index = ( key >> ( step * 2 ) ) & 0x03;

	if( step == 3 ) {
		// We have reached the node nearest to the leaf (next step is the leaf)
		// Deallocate the RAM that this->children[ index ] points to, then NULL the pointer.
		if( current->children[ index ] != NULL ) {
			free( current->children[ index ] );
			current->children[ index ] = NULL;
		}
	} else {
		BitwiseTrieNode* nextChild = current->children[ index ];

		if( nextChild != NULL ) {

			// Recursively call the deleteHelper
			BitwiseTrieNode_deleteHelper( nextChild, current, key, step++ );
		}
	}

	// If all the current child nodes are gone (zero)
	// The parent needs the reference to current zeroed out, and this node needs to be freed
	if( parent != NULL ) {
		size_t i;
		bool allNull = TRUE;
		for( i = 0; i != 4; i++ ) {
			if( current->children[ i ] != NULL ) {
				allNull = FALSE;
				break;
			}
		}

		if( allNull == TRUE ) {
			// This node needs to be freed
			free( current );
			parent->children[ ( key >> ( ( step - 1 ) * 2 ) ) & 0x03 ] = NULL;
		}
	}
}
