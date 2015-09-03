#include <baseview.h>

BaseView_vtable BaseView_table = { BaseView_ctor, BaseView_testa };

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
