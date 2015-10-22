#ifndef RED_BLACK_TREE
#define RED_BLACK_TREE

#include <types.h>

#define	RedBlackTreeNode_RED		0x00
#define RedBlackTreeNode_BLACK		0x01

struct RedBlackTreeNode;

typedef struct RedBlackTreeNode {
	void* data;
	u8 key;

	struct RedBlackTreeNode* leftChild;
	struct RedBlackTreeNode* rightChild;
} RedBlackTreeNode;

void RedBlackTreeNode_ctor( RedBlackTreeNode* this );
void RedBlackTreeNode_dtor( RedBlackTreeNode* this );
void RedBlackTreeNode_insert( RedBlackTreeNode* this, void* data );
void RedBlackTreeNode_delete( RedBlackTreeNode* this, void* data );

#endif
