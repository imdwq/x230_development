#include "config.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

#include "tslib.h"
#include "fbconfig.h"
#include "newtest.h"

static int palette [] =
{
	0x000000, 0xffe080, 0xffffff, 0xe0c0a0, 0x304050, 0x80b8c0
};

#define NR_COLORS (sizeof (palette) / sizeof (palette [0]))
#define NR_BUTTONS 3
static struct ts_button buttons [NR_BUTTONS];

#define X_BLOCK_NUMBER 6
#define Y_BLOCK_NUMBER 3
#define BLOCK_NUMBER (X_BLOCK_NUMBER * Y_BLOCK_NUMBER)

typedef struct ts_block
{
	int x;
	int y;
	int imgid;
	char *on_img_path;
	char *off_img_path;
	char exist_flag;
}Block;

typedef struct ts_block_map
{
	Block Block [BLOCK_NUMBER];
	int pressed_block;
	int size;
	int x_offset;
	int y_offset;
	int block_length;
	int block_height;
	int exist_blocks;
	char reset_flag[BLOCK_NUMBER];
}Block_map;

void get_imgpath(char *string, int i, int state)
{
	if(state == 0)
	{
		sprintf(string, "./bmp/smooth-%d.bmp", i);
	}
	else if(state == 1)
	{
		sprintf(string, "./bmp/smooth-%d.bmp", i + 43);
	}
}

void register_block(Block_map *map, char *off_path, char *on_path)
{
	Block *block = (map->Block) + map->size;
	block->y = map->size / X_BLOCK_NUMBER * map->block_height + map->y_offset;
	block->x = map->size % X_BLOCK_NUMBER * map->block_length + map->x_offset;
	block->imgid = map->size / 2;
	block->on_img_path = on_path;
	block->off_img_path = off_path;
	block->exist_flag = 1;
	(map->size)++;
}

void reset_map(Block_map *map)
{
	int i;
	for(i = 0; i < BLOCK_NUMBER; i++)
		map->Block[i].exist_flag = 1;
	map->exist_blocks = BLOCK_NUMBER;
}

void show_map(Block_map *map)
{
	int i;
	Block *block;
	for(i = 0; i < BLOCK_NUMBER; i++)
	{
		block = (map->Block) + i;
		if(block->exist_flag)
			draw_BMP(block->off_img_path, block->x, block->y);
	}
}

void init_map(Block_map *map)
{
	int i;
	Block *block = map->Block;
	char *string_on = NULL;
	char *string_off = NULL;
	map->pressed_block = -1;
	map->size = 0;
	map->x_offset = 120;
	map->y_offset = 70;
	map->block_length = 96;
	map->block_height = 132;

	map->exist_blocks = BLOCK_NUMBER;
	for(i = 0; i < BLOCK_NUMBER; i++)
	{
		string_off = (char *)malloc(25);
		string_on = (char *)malloc(25);
		map->reset_flag[i] = 0;
		get_imgpath(string_off, i/2, 0);
		get_imgpath(string_on, i/2, 1);
		register_block(map, string_off, string_on);
	}
}

void exit_map(Block_map *map)
{
	int i;
	for(i = 0; i < BLOCK_NUMBER; i++)
	{
		free(map->Block[i].on_img_path);
		free(map->Block[i].off_img_path);
	}
}

int get_block_id(Block_map *map, int x, int y)
{
	int idy = (y - map->y_offset) / map->block_height;
	int idx = (x - map->x_offset) / map->block_length;
	int id = idx + idy * X_BLOCK_NUMBER;
	if(y >= map->y_offset && idy < Y_BLOCK_NUMBER && x >= map->x_offset && idx < X_BLOCK_NUMBER)
	{
		if ((map->Block[id]).exist_flag)
			return id;
	}
	return -1;
}

void press_block(Block_map *map, int id)
{
	Block *block = (map->Block) + id;
	Block *pressed = (map->Block) + map->pressed_block;
	int x = block->x;
	int y = block->y;
	int px = pressed->x;
	int py = pressed->y;

	if (map->pressed_block == id)
	{
		fillrect(x, y, x + map->block_length, y + map->block_height, 0);
		draw_BMP(block->off_img_path, x, y);
		map->pressed_block = -1;
	}
	else if(map->pressed_block == -1)
	{
		fillrect(x, y, x + map->block_length, y + map->block_height, 0);
		draw_BMP(block->on_img_path, x, y);
		map->pressed_block = id;
	}
	else if(block->imgid != pressed->imgid)
	{
		fillrect(px, py, px + map->block_length, py + map->block_height, 0);
		draw_BMP(pressed->off_img_path, px, py);

		fillrect(x, y, x + map->block_length, y + map->block_height, 0);
		draw_BMP(block->on_img_path, x, y);
		map->pressed_block = id;
	}
	else
	{
		fillrect(px, py, px + map->block_length, py + map->block_height, 0);
		fillrect(x, y, x + map->block_length, y + map->block_height, 0);
		map->pressed_block = -1;
		block->exist_flag = 0;
		pressed->exist_flag = 0;
		map->exist_blocks -= 2;
	}
}


void change_map(Block_map *map)
{
	int i;
	int random;
	char *string_on = NULL;
	char *string_off = NULL;
	Block blocktmp[BLOCK_NUMBER];
	if(map->pressed_block >= 0)
		press_block(map, map->pressed_block);
	for(i = 0; i < BLOCK_NUMBER; i++)
	{
	//	free(map->Block[i].on_img_path);
	//	free(map->Block[i].off_img_path);
		blocktmp[i] = map->Block[i];
		map->reset_flag[i] = 1;
	}
	for(i = 0; i < BLOCK_NUMBER; i++)
	{
		random = (rand()) % BLOCK_NUMBER;
		while(!(map->reset_flag[random]))
		{
			random = (random + 1) % BLOCK_NUMBER;
		}
		map->Block[i] = blocktmp[random];
		map->Block[i].y = i / X_BLOCK_NUMBER * map->block_height + map->y_offset;
		map->Block[i].x = i % X_BLOCK_NUMBER * map->block_length + map->x_offset;
	//	string_off = (char *)malloc(25);
	//	string_on = (char *)malloc(25);
	//	get_imgpath(string_off, map->Block[i].imgid, 0);
	//	get_imgpath(string_on, map->Block[i].imgid, 1);
	//	map->Block[i].on_img_path = string_off;
	//	map->Block[i].off_img_path = string_on;
		map->reset_flag[random] = 0;
	}
}

static void sig(int sig)
{
	close_framebuffer();
	fflush(stderr);
	printf("signal %d caught\n", sig);
	fflush(stdout);
	exit(1);
}

static void refresh_screen ()
{
	int i;
	fillrect (0, 0, xres - 1, yres - 1, 0);
	for (i = 0; i < NR_BUTTONS; i++)
		button_draw (&buttons [i]);
}

static void print_help()
{
	int i;
	fillrect (0, 0, xres - 1, yres - 1, 0);
	put_string_center (xres/2, yres/4,"Touch screen to choose the same block", 2);

	for (i = 0; i < NR_BUTTONS; i++)
		button_draw (&buttons [i]);
}

int main()
{
	struct tsdev *ts;
	int x, y;
	unsigned int i;
	unsigned int mode = 0;
	int quit_pressed = 0;
	int newclick_flag = 1;
	char *tsdevice=NULL;

	Block_map map;

	signal(SIGSEGV, sig);
	signal(SIGINT, sig);
	signal(SIGTERM, sig);

	srand(time(NULL));

	if( (tsdevice = getenv("TSLIB_TSDEVICE")) != NULL ) {
		ts = ts_open(tsdevice,0);
	} else {
		if (!(ts = ts_open("/dev/input/event0", 0)))
			ts = ts_open("/dev/touchscreen/ucb1x00", 0);
	}

	if (!ts) {
		perror("ts_open");
		exit(1);
	}

	if (ts_config(ts)) {
		perror("ts_config");
		exit(1);
	}

	if (open_framebuffer()) {
		close_framebuffer();
		exit(1);
	}

	x = xres/2;
	y = yres/2;

	for (i = 0; i < NR_COLORS; i++)
		setcolor (i, palette [i]);

	/* Initialize buttons */
	memset (&buttons, 0,  sizeof (buttons));

	buttons [0].w = buttons [1].w = buttons [2].w = xres / 4;
	buttons [0].h = buttons [1].h = buttons [2].h = 20;
	buttons [0].x = 0;
	buttons [1].x = (3 * xres) / 8;
	buttons [2].x = (3 * xres) / 4;
	buttons [0].y = buttons [1].y = buttons [2].y = 10;
	buttons[0].text = "(Re)start";
	buttons[1].text = "Remap";
	buttons[2].text = "Quit";
	refresh_screen ();

	put_string_center (xres/2, yres/4,"Touch button to start", 2);
	init_map(&map);
	while (1) {
		struct ts_sample samp;
		int ret;

		ret = ts_read(ts, &samp, 1);
		if (ret < 0) {
			perror("ts_read");
			close_framebuffer();
			exit(1);
		}

		if (ret != 1)
			continue;


		for (i = 0; i < NR_BUTTONS; i++)
			if (button_handle(&buttons [i], samp.x, samp.y, samp.pressure))
				switch (i + 1) {
				case 1:
					mode = 1;
					refresh_screen();
					reset_map(&map);
					change_map(&map);
					show_map(&map);
					break;
				case 2:
					mode = 1;
					refresh_screen();
					change_map(&map);
					show_map(&map);
					break;
				case 3:
					quit_pressed = 1;
					exit_map(&map);
					break;
				}


		if (samp.pressure > 0) {
			if(newclick_flag)
			{
				x = samp.x;
				y = samp.y;
				newclick_flag = 0;
			}
			else
			{
				x = (samp.x + x) >> 1;
				y = (samp.y + y) >> 1;
			}
			mode |= 0x80000000;
		} else
			mode &= ~0x80000000;
		if ((mode == 1) || (mode == 0))
		{
			newclick_flag = 1;
			printf("click=(%d,%d)\n", x, y);

			if(mode == 1)
			{
				int id = get_block_id(&map, x, y);
				if (id >= 0)
				{
					press_block(&map, id);
					if(map.exist_blocks == 0)
					{
						put_string_center (xres/2, yres/4,"Congratulations!", 2);
						mode = 0;
					}
				}
			}
		}
		if (quit_pressed)
			break;
	}
	fillrect (0, 0, xres - 1, yres - 1, 0);
	close_framebuffer();

	return 0;
}
