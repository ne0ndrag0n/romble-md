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

void BitwiseTrieNode_ctor( BitwiseTrieNode* this, bool isLeafNode ) {
	if( isLeafNode == TRUE ) {
		this->leftChild.data = NULL;
		this->rightChild.data = NULL;
	} else {
		this->leftChild.node = NULL;
		this->rightChild.node = NULL;
	}
}

void BitwiseTrieNode_dtor( BitwiseTrieNode* this ) {

}

void BitwiseTrieNode_insert( BitwiseTrieNode* this, u8 key, void* value ) {
	BitwiseTrieNode* current = this;

	for( size_t i = 0; i != 8; i++ ) {
		if( ( ( key >> i ) & 0x01 ) == 1 ) {
			// Use right child
		} else {
			// Use left child
		}
	}
}

void BitwiseTrieNode_delete( BitwiseTrieNode* this, u8 key ) {

}

void* BitwiseTrieNode_get( BitwiseTrieNode* this, u8 key ) {

}
