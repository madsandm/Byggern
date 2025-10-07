#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include "uart.h"

extern int uart_putchar(const uint8_t c);

int _write(int fd, const char *buf, int len) {
    (void)fd;
    for (int i = 0; i < len; i++) uart_putchar(buf[i]);
    return len;
}
int _close(int fd){ (void)fd; return -1; }
int _fstat(int fd, struct stat *st){
    (void)fd;
    if (!st) { errno = EINVAL; return -1; }
    st->st_mode = 0;
#ifdef S_IFCHR
    st->st_mode |= S_IFCHR;
#endif
    return 0;
}
int _isatty(int fd){ (void)fd; return 1; }
int _lseek(int fd, int ptr, int dir){ (void)fd; (void)ptr; (void)dir; return 0; }
int _read(int fd, char *buf, int len){ (void)fd; (void)buf; (void)len; return 0; }

/* Only needed if you use malloc/new; simplest is to stub it out to fail cleanly.
   Provide a real _sbrk later if you need heap. */
void* _sbrk(ptrdiff_t incr){
    errno = ENOMEM;
    return (void*)-1;
}