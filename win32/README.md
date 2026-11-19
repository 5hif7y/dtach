# dtach for Windows (native MSVC)

Native port of dtach: no Cygwin, MSYS or WSL. Sessions run inside a ConPTY
pseudoconsole and are reached over a Winsock AF_UNIX socket.

## Build and test

```bat
win32\build.bat
powershell -ExecutionPolicy Bypass -File win32\tests\run-tests.ps1
```

Requires Windows 10 1809+ (ConPTY), Visual Studio with C++ and CMake 3.20+.

## Usage (same as upstream)

```bat
dtach -A %TEMP%\s1 cmd.exe     :: create or attach; Ctrl+\ detaches
dtach -a %TEMP%\s1             :: attach again (ConPTY repaints the screen)
dtach -n %TEMP%\s2 powershell  :: create detached
echo dir | dtach -p %TEMP%\s1  :: push input
```

On non-US keyboards Ctrl+\ may be impossible to type: use `-e '^q'`.

## Windows differences

- Default redraw method is `winch`: ConPTY keeps the whole screen and
  repaints it on resize, so reattaching always redraws.
- No job control: the suspend key is disabled.
- The detached master is the same `dtach.exe` re-executed with
  `DETACHED_PROCESS` (no `fork()`); errors reach the client through a pipe.

## Layout

```
win32/compat/   POSIX-on-Win32 layer (virtual fds, select over
                WaitForMultipleObjects, emulated signals, ConPTY,
                AF_UNIX, termios on the console). Upstream code never
                sees <windows.h>: compat.h is force-included (/FI).
win32/tests/    ptyrun.c (scripted ConPTY terminal) + run-tests.ps1
```

All source changes are under `#ifdef _WIN32` and tagged `WIN32:`; see
`git diff b027c27..port/windows -- . ':!win32'`.

Debugging a detached server: `set COMPAT_TRACE=C:\path\trace.log`.

The port and its write-up come from the N5-multiplexers kata of
[LowLevel-Katas](https://github.com/5hif7y/LowLevel-Katas).
