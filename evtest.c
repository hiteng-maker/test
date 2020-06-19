#include <stdint.h>
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#ifndef EV_SYN
#define EV_SYN 0
#endif

#define BITS_PER_LONG (sizeof(long) * 8)
#define OFF(x) ((x)%BITS_PER_LONG)
#define BIT(x) (1UL << OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define test_bit(bit,array)  ((array[LONG(bit)] >> OFF(bit)) & 1)

char *events[EV_MAX + 1]={
	[EV_SYN]= "sync",
	[EV_KEY]= "key",
};

char *keys[KEY_MAX + 1] = {
	[0 ... KEY_MAX] = NULL,
	[BTN_0] = "Btn0",
};

char **names[EV_MAX + 1]= {
	[0 ... EV_MAX] = NULL,
	[EV_SYN] = events,
	[EV_KEY] = keys,
};

int main( int argc , char **argv)
{
	int fd , rd, i,j;
	struct input_event ev[64];
	int version;
	unsigned short id[4];
	unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
	char name[256]= "unknown";


	if((fd = open(argv[argc - 1],O_RDONLY))< 0)
	{	
		perror("evtest");
		return 1;
	}
	
	ioctl(fd , EVIOCGVERSION, &version);

	printf("input driver version is %d.%d.%d\n",
			version >> 16, (version>>8) & 0xff, version & 0xff);
	
	memset(bit,0, sizeof(bit));

	ioctl(fd , EVIOCGBIT(0, EV_MAX), bit[0]);
	
	printf("supported events:\n");	

	for (i = 0 ; i< EV_MAX ; i++)
		if (test_bit(i, bit[0]))
		{
			printf("Event type %d (%s)\n",i , events[i] ? events[i]: "?");
			if (!i) continue;
			ioctl(fd, EVIOCGBIT(i, KEY_MAX), bit[i]);

			for (j=0 ; j < KEY_MAX; j++)
				if (test_bit(j, bit[i]))
				{
					printf("Event code %d (%s)\n", j, names[i] ? (names[i][j] ? names[i][j] : "?") : "?");
				}
		}

	printf("Testing.....\n");

	while(1)
	{
		rd = read(fd, ev, sizeof(struct input_event) * 64);

		if(rd< (int) sizeof(struct input_event))
		{
			printf("Error in reading\n");
			return 1;
		}

		for(i=0; i < rd/sizeof(struct input_event);i++)
			if(ev[i].type == EV_SYN)
			{
				printf("Event: time %ld.%06ld, -----------%s------------\n",ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].code ? "config sync" : "report sync" );
			}
			else 
			{
				printf("Event: time %ld.%06ld, type %d (%s),code %d (%s),value %d\n", ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type, events[ev[i].type] ? events[ev[i].type] : "?", ev[i].code, names[ev[i].type] ? (names[ev[i].type][ev[i].code] ? names[ev[i].type][ev[i].code] : "?") : "?", ev[i].value);
			}
	}
}

