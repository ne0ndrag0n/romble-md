/**
 * Bitwise Trie. This trie supports
 * keys of u8 type, and any type
 * of value.
 *
 * @author		Ash
 * @version		0.0.1
 */
#ifndef		BIT_TRIE
#define		BIT_TRIE

#include <types.h>
#include <stddef.h>
#include <res/globals.h>
#include <sizedarray.h>

#define	 BitwiseTrieNode_CELL_MASK	0x03

typedef union BitwiseTrieNode {
	void* data;
	union BitwiseTrieNode** children;
} BitwiseTrieNode;

typedef struct BitwiseTrie {
	BitwiseTrieNode* root;
	SizedArray keys;
} BitwiseTrie;

void BitwiseTrieNode_ctor( BitwiseTrieNode* this, bool leaf );
void BitwiseTrieNode_dtor( BitwiseTrieNode* this );

void BitwiseTrieNode_insert( BitwiseTrieNode* this, u8 key, void* value );
void* BitwiseTrieNode_get( BitwiseTrieNode* this, u8 key );

void BitwiseTrieNode_delete( BitwiseTrieNode* this, u8 key );

void BitwiseTrieNode_deleteHelper( BitwiseTrieNode* current, BitwiseTrieNode* parent, u8 key, u8 step );
extern u8 BitwiseTrieNode_getCell( u8 key, u8 step );

// ------- BitwiseTrie container (wraps a BitwiseTrieNode) -------

void BitwiseTrie_ctor( BitwiseTrie* this );
void BitwiseTrie_dtor( BitwiseTrie* this );

void BitwiseTrie_insert( BitwiseTrie* this, u8 key, void* data );
void* BitwiseTrie_get( BitwiseTrie* this, u8 key );
void BitwiseTrie_delete( BitwiseTrie* this, u8 key );

#endif
