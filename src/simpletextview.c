#include <simpletextview.h>

SimpleTextView_vtable SimpleTextView_table = { SimpleTextView_ctor, BaseView_testa };

void SimpleTextView_ctor( SimpleTextView* this, char* text, u8 x, u8 y, u8 width, u8 height ) {
	BaseView_ctor( this, x, y, width, height );

	this->super.functions = &SimpleTextView_table;
} 
