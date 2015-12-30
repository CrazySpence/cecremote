#include <stdlib.h>
#include <stdio.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int cecsendkey(int fd, char key);

int main(int argc, char *argv[]) {
	int  fd;
	int  ret;
	struct uinput_user_dev uidev;
	char incoming[512];
	FILE *cec_client;
	int pressed = 0; //Key prssed status, 0 no 1 yes
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
	ret = ioctl(fd, UI_SET_KEYBIT, KEY_TAB);

	memset(&uidev, 0, sizeof(uidev));
        snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "cecremote");
        uidev.id.bustype = BUS_USB;
        uidev.id.vendor  = 0x1;
        uidev.id.product = 0x1;
        uidev.id.version = 1;

	ret = write(fd, &uidev, sizeof(uidev));
	ret = ioctl(fd, UI_DEV_CREATE);

	if(!(cec_client = popen("cec-client","r"))) {
		printf("ERROR: could not open cec-client\n");
		return -1;
	}
	while(1) {
		fgets(incoming,512,cec_client); //read data when available
		if(pressed == 0) {
			if(strstr(incoming,"key pressed: up")) {
				cecsendkey(fd,'U'); 
				pressed = 1;
			}
			if(strstr(incoming,"key pressed: down")) {
				cecsendkey(fd,'D');
				pressed = 1;
			}
			if(strstr(incoming,"key pressed: left")) {
				cecsendkey(fd,'L');
				pressed = 1;
			}
			if(strstr(incoming,"key pressed: right")) {
				cecsendkey(fd,'R');
				pressed = 1;
			}
			if(strstr(incoming,"key pressed: exit")) {
				cecsendkey(fd,'C');
				pressed = 1;
			}
			if(strstr(incoming,"key pressed: select")) {
				cecsendkey(fd,'E');
				pressed = 1;
			}
			if(strstr(incoming,"key pressed: 0")) {
				cecsendkey(fd,'0');
				pressed = 1;
			}
		}
		if(strstr(incoming,"key released")) {
                        pressed = 0;
                }
	}
	return 0;

}

int cecsendkey(int fd, char key) {
 	int ret;
	struct input_event     synev,ev;
	memset(&ev, 0, sizeof(ev));
        ev.type = EV_KEY;

	switch(key) {
		case 'U':
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
		case '0':
			ev.code = KEY_TAB; //PiPlay Menu key
		default:
		break;
	}
	ev.value = 1;
	ret = write(fd, &ev, sizeof(ev)); //Send Key
	usleep(10000);
	memset(&synev, 0, sizeof(synev));
	synev.type = EV_SYN;
	synev.code = SYN_REPORT;
	synev.value = 0;
 	ret = write(fd, &synev, sizeof(synev)); //Send Key
	usleep(10000);

	//depress key
	ev.value = 0;
	ret = write(fd, &ev, sizeof(ev)); //Send Key
        usleep(10000);
        ret = write(fd, &synev, sizeof(synev)); //Send Key
	usleep(10000);

	return 0;
}

int clear_device(int fd) {
	int ret;
	ret = ioctl(fd, UI_DEV_DESTROY); //clean device
        sleep(1);
	close(fd);
	return 0;
}
