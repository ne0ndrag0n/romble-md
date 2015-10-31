/**
 * Bitwise Trie cfile
 *
 * @author		Ash
 * @version		0.0.1
 */

#include <bit_trie.h>
#include <types.h>
#include <stddef.h>

void BitwiseTrieNode_ctor( BitwiseTrieNode* this ) {
	this->data = NULL;

	this->zero = NULL;
	this->one = NULL;
}

void BitwiseTrieNode_dtor( BitwiseTrieNode* this ) {

}

void BitwiseTrieNode_insert( BitwiseTrieNode* this, u8 key, void* value ) {
	BitwiseTrieNode* current = this;

	for( size_t i = 0; i != 8; i++ ) {
		if( ( ( key >> i ) & 0x01 ) == 1 ) {

		} else {

		}
	}
}

void BitwiseTrieNode_delete( BitwiseTrieNode* this, u8 key ) {

}

void* BitwiseTrieNode_get( BitwiseTrieNode* this, u8 key ) {

}
