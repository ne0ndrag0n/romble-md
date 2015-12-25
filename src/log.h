#ifndef SYSTEM_LOG
#define SYSTEM_LOG

#define		Log_LEVEL			0

#define		Log_ROOT_LEVEL		0
#define		Log_MESSAGE_BUFFER	251
#define		Log_FORMAT_STRING	"%10lu | %c | %-30.30s | %.99s"

typedef enum {
	Log_Level_DEBUG = Log_ROOT_LEVEL,
	Log_Level_INFO,
	Log_Level_WARNING,
	Log_Level_ERROR
} Log_Level;

void Log_message( Log_Level level, char* module, char* message );
void Log_fmessage( Log_Level level, char* module, char* format, ... );

#endif
