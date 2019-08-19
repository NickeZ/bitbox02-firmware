#include <unistd.h>
#include <string.h>
#include <ui/fonts/arial_fonts.h>
#include <hal_delay.h>

#include "screen.h"
#include "util.h"

int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void _ttywrch(int ch);

int _write (int fd, char *ptr, int len)
{
    if(len < 0) {
        return 0;
    }
    if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
        while(len > 0) {
            char print[120];
            int plen = MIN(sizeof(print)-1, (unsigned)len); /* Actual length written */
            memcpy(print, ptr, plen);
            print[plen] = '\0';
            UG_ClearBuffer();
            UG_FontSelect(&font_font_a_9X9);
            UG_PutString(0, 0, print, false);
            UG_SendBuffer();
            len -= plen;
            ptr += plen;
        }
    }
    return len;
}

int _read (int fd, char *ptr, int len)
{
    (void)fd;
    (void)ptr;
    (void)len;
  /* Read "len" of char to "ptr" from file id "fd"
   * Return number of char read.
   * Need implementing with UART here. */
  return len;
}

void _ttywrch(int ch) {
    char print[2];
    print[0] = ch;
    print[1] = '\0';
    UG_ClearBuffer();
    UG_FontSelect(&font_font_a_9X9);
    UG_PutString(0, 0, print, false);
    UG_SendBuffer();
    delay_ms(500);
}
