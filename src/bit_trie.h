/**
 * Bitwise Trie. This trie supports
 * keys of u8 type, and any type
 * of value.
 *
 * @author		Ash
 * @version		0.0.1
 */

#include <types.h>
#include <res/globals.h>

#ifndef		BIT_TRIE
#define		BIT_TRIE

typedef union BitwiseTrieNode {
	void* data;
	union BitwiseTrieNode** children;
} BitwiseTrieNode;

void BitwiseTrieNode_ctor( BitwiseTrieNode* this );
void BitwiseTrieNode_dtor( BitwiseTrieNode* this );

void BitwiseTrieNode_insert( BitwiseTrieNode* this, u8 key, void* value );

#endif
