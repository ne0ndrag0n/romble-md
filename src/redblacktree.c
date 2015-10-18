#include <redblacktree.h>
#include <romble.h>

void RedBlackTreeNode_ctor( RedBlackTreeNode* this ) {
	this->data = NULL;
	this->key = 0;

	this->leftChild = NULL;
	this->rightChild = NULL;
}

void RedBlackTreeNode_dtor( RedBlackTreeNode* this ) {
	if( this->leftChild != NULL ) {
		RedBlackTreeNode_dtor( this->leftChild );
	}

	if( this->rightChild != NULL ) {
		RedBlackTreeNode_dtor( this->rightChild );
	}

	Romble_secureFree( ( void* ) &( this->leftChild ) );
	Romble_secureFree( ( void* ) &( this->rightChild ) );
}

void RedBlackTreeNode_insert( RedBlackTreeNode* this, void* data ) {

}

void RedBlackTreeNode_delete( RedBlackTreeNode* this, void* data ) {

}
