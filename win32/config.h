/* config.h for dtach on Windows/MSVC (replaces autoconf's output). */
#define PACKAGE_VERSION "0.9-win32"
#define PACKAGE_BUGREPORT "crigler@gmail.com"
#define RETSIGTYPE void
#define HAVE_UNISTD_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_FORKPTY 1   /* we never compile the BSD fallbacks */
#define HAVE_OPENPTY 1
