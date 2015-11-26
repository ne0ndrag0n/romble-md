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

void BitwiseTrieNode_ctor( BitwiseTrieNode* this ) {
	// Allocate four child elements; they will be zeroed out
	this->children = Romble_alloc_d( sizeof( BitwiseTrieNode* ) * 4, TRUE, FILE_LINE() );
}

void BitwiseTrieNode_dtor( BitwiseTrieNode* this ) {
	// Recursively remove all elements
}

void BitwiseTrieNode_insert( BitwiseTrieNode* this, u8 key, void* value ) {
	BitwiseTrieNode* current = this;

	size_t i;
	for( i = 0; i != 3; i++ ) {
		u8 index = ( key >> ( i * 2 ) ) & 0x03;


	}

	// Current should now be the last one in the chain
}
