#include <simpletextview.h>
#include <baseview.h>
#include <string.h>
#include <vdp_bg.h>

SimpleTextView_vtable SimpleTextView_table = { SimpleTextView_ctor, SimpleTextView_testa, SimpleTextView_render };

void SimpleTextView_ctor( SimpleTextView* this, char* text, u8 x, u8 y ) {
	u8 width = strlen( text );
	
	BaseView_ctor( &this->super, x, y, width, 1 );

	// Fix the incompatible pointer types warning
	this->super.functions = &SimpleTextView_table;
}

void SimpleTextView_render( SimpleTextView* this ) {
	BaseView_render( &this->super );

	VDP_drawText( this->text, this->super.absX, this->super.absY );
}

u8 SimpleTextView_testa( SimpleTextView* this ) {
	u8 result = BaseView_testa( ( BaseView* ) this );

	return result + 200;
}
