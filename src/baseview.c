#include <baseview.h>

BaseView_vtable BaseView_table = { BaseView_ctor, BaseView_testa, BaseView_render };

void BaseView_ctor( BaseView* this, u8 x, u8 y, u8 width, u8 height ) {
	this->functions = &BaseView_table;

	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;
}

u8 BaseView_testa( BaseView* this ) {
	return this->width + this->height;
}

void BaseView_render( BaseView* this ) {
	// If this view has no parent, set absX and absY to zero....else, add the absX and absY of the parent
	if( this->parent == NULL ) {
		this->absX = this->absY = 0;
	} else {
		this->absX = this->x + this->absX;
		this->absY = this->y + this->absY;
	}
}
