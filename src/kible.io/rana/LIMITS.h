#ifndef LIMITS_H_
#define LIMITS_H_

//#define BUILD_UI_LOCALLY
//#define TESTING_BUILD

#define SIZE_OF_RANA_IDENTIFIER 20
#define SOFTWARE_VERSION "3.4.1" //poseidon
#define BYTES_PER_PIXEL 4
#define THEMIS_PORT 4460
#define THEMIS_PORT_HTTP 4461
#define FPS 120
#define SCALE_THRESHOLD 1500000
#define SCALE_RATIO 2.0 / 3.0

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

#define MINIMUM_WIDTH 480
#define MINIMUM_HEIGHT 480

#define MAXIMUM_WIDTH 1920
#define MAXIMUM_HEIGHT 1080

#define THEMIS_INFO_PATH "Info.conf"
#define __CORE_SYSTEM__ "RANA"

// Linux specific code {{{
#ifdef __linux__
#define RANA_ASSET_PATH "/root/RANA/res/"
#define RANA_IDENTIFIER_LOCATION "/opt/identifier"
#endif
// }}} Windows specific code {{{
#ifdef _WIN64
#define RANA_ASSET_PATH "./res/"
#define RANA_IDENTIFIER_LOCATION "./identifier"
#endif
// }}} OSX specific code {{{
#ifdef __APPLE__
// TODO apple code
#endif
// }}}
#define RANA_ASSET(A) (RANA_ASSET_PATH + string(A))

/*======= Rana update data ========*/
#define PATH_TO_VERSION "/beta/rana/version.html"

// Define the remote site with the latest executable
// for the ARM and for x86
#ifdef __arm__
#define REMOTE_FILE_ARCHIVE_NAME "rana_arm.tar.gz"
#else
#define REMOTE_FILE_ARCHIVE_NAME "rana.tar.gz"	//"rana_x86_64.tar.gz"
#endif

#define PATH_TO_FILE_ARCHIVE (string("/beta/rana/") + REMOTE_FILE_ARCHIVE_NAME)
/*======= Rana update data ========*/

#endif
