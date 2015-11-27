/**
 * Bitwise Trie cfile
 *
 * @author		Ash
 * @version		0.0.1
 */

#include <bit_trie.h>
#include <types.h>
#include <stddef.h>
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
