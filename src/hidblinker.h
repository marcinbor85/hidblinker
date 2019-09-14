#ifndef _HIDBLINKER_H
#define _HIDBLINKER_H

struct hidblinker;

struct hidblinker* hidblinker_init(void);
void hidblinker_close(struct hidblinker *dev);

int hidblinker_toggle_led(struct hidblinker *dev);
int hidblinker_read_button(struct hidblinker *dev, char *state);

#endif
