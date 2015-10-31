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

struct BitwiseTrieNode;

typedef union BitwiseTrieNodeChild {
	void* data;
	struct BitwiseTrieNode* node;
} BitwiseTrieNodeChild;

typedef struct BitwiseTrieNode {
	/**
	 * Left holds 0, right holds 1
	 */
	BitwiseTrieNodeChild leftChild;
	BitwiseTrieNodeChild rightChild;
} BitwiseTrieNode;

void BitwiseTrieNode_ctor( BitwiseTrieNode* this, bool isLeafNode );
void BitwiseTrieNode_dtor( BitwiseTrieNode* this );

void BitwiseTrieNode_insert( BitwiseTrieNode* this, u8 key, void* value );
void BitwiseTrieNode_delete( BitwiseTrieNode* this, u8 key );

void* BitwiseTrieNode_get( BitwiseTrieNode* this, u8 key );

#endif
