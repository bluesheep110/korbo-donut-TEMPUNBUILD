#ifndef MUSIC_H
#define MUSIC_H

extern void music_play(char *filename);
extern void music_init(char *title_display);
extern void music_tick();
extern void music_pause(const bool pause);
extern void music_free(void);

#endif
