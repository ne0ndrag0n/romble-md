/**
 * oaktree MdVC BaseView
 * (c) 2015 Oaktree Games and Novelties
 */
#include <baseview.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys.h>
#include <lang.h>
#include <res/globals.h>
#include <utility.h>

BaseView_vtable BaseView_table = { 
	BaseView_ctor, 
	BaseView_testa, 
	BaseView_render,
	BaseView_addChildView
};

void BaseView_ctor( BaseView* this, u8 x, u8 y, u8 width, u8 height ) {
	this->functions = &BaseView_table;

	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;

	this->children = this->parent = NULL;
	this->numChildren = 0;
}

u8 BaseView_testa( BaseView* this ) {
	return this->width + this->height;
}

void BaseView_render( BaseView* this, bool skipChildren ) {
	// If this view has no parent, set absX and absY to where the view is placed....else, add the absX and absY of the parent
	if( this->parent == NULL ) {
		this->absX = this->x;
		this->absY = this->y;
	} else {
		this->absX = this->x + this->parent->absX;
		this->absY = this->y + this->parent->absY;
	}
	
	// Place and render children
	if( skipChildren == FALSE && this->children != NULL ) {
		size_t i;

		for( i = 0; i != this->numChildren; i++ ) {
			BaseView* view = this->children[ i ];

			view->functions->render( view, skipChildren );
		}
	}
}

void BaseView_addChildView( BaseView* this, BaseView* childView ) {
	// Create the children collection if it does not exist
	if( this->children == NULL ) {
		this->children = ( BaseView** ) malloc( sizeof( BaseView* ) );
		
		if( this->children == NULL ) {
			SYS_die( EXCEPTION_OUT_OF_MEMORY );
		}

		this->numChildren++;
	} else {

		this->numChildren++;
		BaseView** resizedArray = ( BaseView** ) realloc( this->children, this->numChildren );
		if( resizedArray == NULL ) {
			SYS_die( EXCEPTION_OUT_OF_MEMORY );
		} else {
			this->children = resizedArray;
		}
	}
	
	// After resized, set the last elemnt in the array to childView
	childView->parent = this;
	this->children[ this->numChildren - 1 ] = childView;
}
