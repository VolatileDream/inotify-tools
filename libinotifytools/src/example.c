#include <stdio.h>
#include <string.h>
#include <inotifytools/inotifytools.h>
#include <inotifytools/inotify.h>

#define LOOK_FOR ( IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVE_SELF )

/*
 * libinotifytools example program.
 * Compile with gcc -linotifytools example.c
 */
int main(int argc, char** argv) {

	char * path = ".";
	if( argc > 1 ){
		path = argv[1];
	}

	// initialize and watch the entire directory tree from the current working
	// directory downwards for all events
	if ( !inotifytools_initialize()
	  || !inotifytools_watch_recursively( path, LOOK_FOR ) ) {
		fprintf(stderr, "%s\n", strerror( inotifytools_error() ) );
		return -1;
	}

	// set time format to 24 hour time, HH:MM:SS
	inotifytools_set_printf_timefmt( "%T" );

	// Output all events as "<timestamp> <path> <events>"
	struct inotify_event * event = inotifytools_next_event( -1 );
	while ( event ) {
		inotifytools_printf( event, "%w%f\n" );
		fflush(stdout); // this fixes all issues with working with pipes
		event = inotifytools_next_event( -1 );
	}
}

