#include <stdlib.h>
#include <stdio.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int  fd;
	int  ret;
	char *input;
	char key;
	struct uinput_user_dev uidev;
    	struct input_event     ev;

	if(argc < 2 ) {
		printf("You must specify a key value");
		exit(0);
	}

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if(fd < 0) {
		printf("Some kind of error\n");
    		exit(EXIT_FAILURE);
	}

	ret = ioctl(fd, UI_SET_EVBIT, EV_KEY);
	ret = ioctl(fd, UI_SET_EVBIT, EV_SYN);

//Set Keys
	ret = ioctl(fd, UI_SET_KEYBIT, KEY_UP);
        ret = ioctl(fd, UI_SET_KEYBIT, KEY_DOWN);
        ret = ioctl(fd, UI_SET_KEYBIT, KEY_LEFT);
	ret = ioctl(fd, UI_SET_KEYBIT, KEY_RIGHT);
	ret = ioctl(fd, UI_SET_KEYBIT, KEY_ENTER);
	ret = ioctl(fd, UI_SET_KEYBIT, KEY_ESC);

	memset(&uidev, 0, sizeof(uidev));
        snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-cecremote");
        uidev.id.bustype = BUS_USB;
        uidev.id.vendor  = 0x1;
        uidev.id.product = 0x1;
        uidev.id.version = 1;

	ret = write(fd, &uidev, sizeof(uidev));
	ret = ioctl(fd, UI_DEV_CREATE);

	memset(&ev, 0, sizeof(ev));
        ev.type = EV_KEY;
	input = argv[1];
	key = input[0];

	switch(key) {
		case 'U':
                        printf("Up\n");
			ev.code = KEY_UP;
		break;
		case 'D':
			ev.code = KEY_DOWN;
		break;
		case 'L':
			ev.code = KEY_LEFT;
		break;
		case 'R':
			ev.code = KEY_RIGHT;
		break;
		case 'E':
			ev.code = KEY_ENTER;
		break;
		case 'C':
			ev.code = KEY_ESC;
		break;
		default:
		break;
	}
	ev.value = 1;
	ret = write(fd, &ev, sizeof(ev)); //Send Key

	memset(&ev, 0, sizeof(ev));
	ev.type = EV_SYN;
	ev.code = 0;
	ev.value = 0;
 	ret = write(fd, &ev, sizeof(ev)); //Send Key
	usleep(10000);

	ret = ioctl(fd, UI_DEV_DESTROY); //clean device
        sleep(1);
	close(fd);
	return 0;
}
