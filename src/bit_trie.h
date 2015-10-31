/**
 * Bitwise Trie. This trie supports
 * keys of u8 type, and any type
 * of value.
 *
 * @author		Ash
 * @version		0.0.1
 */

#include <types.h>

#ifndef		BIT_TRIE
#define		BIT_TRIE

struct BitwiseTrieNode;

typedef struct BitwiseTrieNode {
	void* data;

	struct BitwiseTrieNode* zero;
	struct BitwiseTrieNode* one;
} BitwiseTrieNode;

void BitwiseTrieNode_ctor( BitwiseTrieNode* this );
void BitwiseTrieNode_dtor( BitwiseTrieNode* this );

void BitwiseTrieNode_insert( BitwiseTrieNode* this, u8 key, void* value );
void BitwiseTrieNode_delete( BitwiseTrieNode* this, u8 key );

void* BitwiseTrieNode_get( BitwiseTrieNode* this, u8 key );

#endif
