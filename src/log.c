#include <log.h>
#include <types.h>
#include <timer.h>
#include <utility.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void Log_message( Log_Level level, char* module, char* message ) {
	// Verify the current log level is equal to or less than this level
	if( Log_LEVEL <= level ) {
		// Static arrays for security
		char messageBuffer[ Log_MESSAGE_BUFFER ] = { 0 };

		char levelChar = '?';
		switch( level ) {
			case Log_Level_DEBUG:
				levelChar = 'D';
				break;
			case Log_Level_INFO:
				levelChar = 'I';
				break;
			case Log_Level_WARNING:
				levelChar = 'W';
				break;
			case Log_Level_ERROR:
				levelChar = 'E';
				break;
		}

		snprintf( messageBuffer, Log_MESSAGE_BUFFER - 1, Log_FORMAT_STRING,
			getTick(),
			levelChar,
			module,
			message
		);

		Debug_print( messageBuffer );
	}
}

void Log_fmessage( Log_Level level, char* module, char* format, ... ) {
	va_list args;
	va_start( args, format );

	char string[ 100 ];
	vsnprintf( string, 99, format, args );
	va_end( args );

	Log_message( level, module, string );
}
