#include <simpletextview.h>
#include <baseview.h>
#include <vdp_bg.h>

SimpleTextView_vtable SimpleTextView_table = { SimpleTextView_ctor, BaseView_testa, SimpleTextView_render };

void SimpleTextView_ctor( SimpleTextView* this, char* text, u8 x, u8 y, u8 width, u8 height ) {
	BaseView_ctor( &this->super, x, y, width, height );

	// Fix the incompatible pointer types warning
	this->super.functions = &SimpleTextView_table;
}

void SimpleTextView_render( SimpleTextView* this ) {
	BaseView_render( &this->super );

	VDP_drawText( this->text, this->super.absX, this->super.absY );
}
