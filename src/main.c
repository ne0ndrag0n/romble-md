#include <types.h>
#include <vdp_bg.h>
#include <stdio.h>
#include <baseview.h>
#include <simpletextview.h>
#include <boxview.h>
#include <utility.h>
#include <res/globals.h>
#include <vdp_pal.h>
#include <vdp_tile.h>
#include <romble.h>
#include <joymanager.h>
#include <testview.h>
#include <res/globals.h>

int main( void ) {

	Romble_init();

	BaseView root;
	BaseView_ctor( &root, 0, 0, 40, 28 );

	TestView test;
	TestView_ctor( &test, 2, 2, 35, 25 );
	
	FUNCTIONS( BaseView, BaseView, &root )->addChildView( &root, CLASS( BaseView, &test ) );
	FUNCTIONS( BaseView, BaseView, &root )->render( &root );

	Debug_print( "Romble "__DATE__" "__TIME__ );

	while(1);
}
