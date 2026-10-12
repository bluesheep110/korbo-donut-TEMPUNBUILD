#include <stdlib.h>
#include <ogc/conf.h>
#include <string.h>
#include <grrmod-xmp.h>

#include <aesndlib.h>

#include "music.h"

#include "file.h"
#include "text.h"

#include "music_it.h"

#define FILETYPE_COUNT 68

static bool paused = false;
static u8 *module = NULL;
static FileList *list = NULL;

static bool is_title_empty(const char *title) {
	if (title[0] == '\0') return true;

	const int length = strlen(title) - 1;
	bool isAllSpaces = false;
	for (int i = 0; i < length; i++) {
		isAllSpaces |= (title[i] == ' ');
	}
	return isAllSpaces;
}

int music_init(char *title_display) {
	list = create_file_list();

	file_find(list, "music/", "*", FNM_PATHNAME);
	FILE *f = NULL;
	if (list->filenames[0]) {
		f = file_open(list->filenames[0], "rb");
	}

	GRRMOD_Init(SYS_GetSoundMode()); // check for mono
	if (f) {
		fseek(f, 0, SEEK_END);
		long module_size = ftell(f);
		rewind(f);

		module = (u8 *)malloc(module_size);

		fread(module, 1, module_size, f);

		GRRMOD_SetMOD(module,module_size);
		file_close(f);
	} else { GRRMOD_SetMOD(music_it, music_it_size); }

	char *title = GRRMOD_GetSongTitle();
	format_info("Song: ", is_title_empty(title) ? "Untitled" : title, title_display, true);
	// format_info("Song: ", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", title_display);

	GRRMOD_Start();
	return strlen(title_display);
}

void music_free(void) {
	if (module)
		free(module);
	GRRMOD_End();
}

void music_pause(const bool pause) {
	if (pause != paused) {
		GRRMOD_Pause();
		paused = !paused;
	}
}
