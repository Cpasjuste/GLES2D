#ifdef _PANDORA_

#include "GLES2D/GLES2D_control.h"
#include "GLES2D_control_pnd.h"

char event_name[30];
int fd_usbk, fd_usbm, fd_gpio, fd_pndk, fd_nub1, fd_nub2, fd_ts, rd, i, j, k;
struct input_event ev[64];
int version;
unsigned short id[4];
unsigned long bit[EV_MAX][NBITS(KEY_MAX)];
char dev_name[256] = "Unknown";
int absolute[5];

char pnd_nub1[9]  = "vsense66";
char pnd_nub2[9]  = "vsense67";
char pnd_key[19]  = "omap_twl4030keypad";
char pnd_gpio[10] = "gpio-keys";

#define DEV_NUB1 0
#define DEV_NUB2 1
#define DEV_PNDK 2
#define DEV_GPIO 3

#define NUB1_CUTOFF 100
#define NUB2_CUTOFF 100
#define NUB2_SCALE  10

void PND_Setup_Controls( void )
{
	gprintf( "Setting up Pandora Controls\n" );

	// Static Controls
	// Pandora keyboard
	fd_pndk = PND_OpenEventDeviceByName(pnd_key);
	// Pandora buttons
	fd_gpio = PND_OpenEventDeviceByName(pnd_gpio);
	// Pandora analog nub's
	fd_nub1 = PND_OpenEventDeviceByName(pnd_nub1);
	fd_nub2 = PND_OpenEventDeviceByName(pnd_nub2);
}

void PND_Close_Controls( void )
{
	gprintf( "Closing Pandora Controls\n" );

	if( fd_pndk > 0 )
		close(fd_pndk );
	if( fd_gpio > 0 )
		close(fd_gpio );
	if( fd_nub1 > 0 )
		close(fd_nub1 );
	if( fd_nub2 > 0 )
		close(fd_nub2 );
}

void PND_SendKeyEvents( )
{
	PND_ReadEvents( fd_pndk, DEV_PNDK );
	PND_ReadEvents( fd_gpio, DEV_GPIO );
	PND_ReadEvents( fd_nub1, DEV_NUB1 );
	PND_ReadEvents( fd_nub2, DEV_NUB2 );
}



void PND_ReadEvents( int fd, int device )
{
	if( fd != 0 )
	{
		rd = read(fd, ev, sizeof(struct input_event) * 64);

		if (rd > (int) sizeof(struct input_event))
		{
			for (i = 0; i < rd / sizeof(struct input_event); i++)
			{
				PND_CheckEvent( &ev[i], device );
			}
		}
	}
}

void PND_CheckEvent( struct input_event *event, int device )
{
	int value;

	gprintf( "Device %d Type %d Code %d Value %d\n", device, event->type, event->code, event->value );

	value	= event->value;
	switch( event->type )
	{
		case EV_KEY:
			switch( event->code )
			{
				case 103: // UP
					GLES2D_Pad[PAD_UP] = event->value;
				break;

				case 108: // DOWN
					GLES2D_Pad[PAD_DOWN] = event->value;
				break;

				case 105: // LEFT
					GLES2D_Pad[PAD_LEFT] = event->value;
				break;

				case 106: // RIGHT
					GLES2D_Pad[PAD_RIGHT] = event->value;
				break;

				case 139: // MENU
					GLES2D_Pad[MENU] = event->value;
				break;

				case 56: // START
					GLES2D_Pad[START] = event->value;
				break;

				case 29: // SELECT
					GLES2D_Pad[SELECT] = event->value;
				break;

				case 81: // X
					GLES2D_Pad[X] = event->value;
				break;

				case 79: // Y
					GLES2D_Pad[Y] = event->value;
				break;

				case 75: // A	
					GLES2D_Pad[A] = event->value;
				break;

				case 80: // B
					GLES2D_Pad[B] = event->value;
				break;

				case 76:
					GLES2D_Pad[L] = event->value;
				break;

				case 77:
					GLES2D_Pad[R] = event->value;
				break;

				default:
					break;
			}
			break;

		case EV_ABS:
			switch( device )
			{
				case DEV_NUB1:
					if( event->code == ABS_X )
					{
						//printf( "nub1 x %3d\n", value );
						if( abs(value) > NUB1_CUTOFF ) {
							if( value > 0 ) {
								value = 1;
							}
							else if( value < 0 ) {
								value = 1;						    
							}
						}
						else {

						}
					}

					if( event->code == ABS_Y ) {
						//printf( "nub1 y %3d\n", value );
						if( abs(value) > NUB1_CUTOFF ) {
							if( value > 0 ) {
								value = 1;
							}
							else if( value < 0 ) {
								value = 1;
							}
						}
						else {

						}

					}
					break;
				case DEV_NUB2:
					if(event->code == ABS_X)
					{
						//printf( "nub2 x %3d\n", value );
						if( abs(value) > NUB2_CUTOFF ) {
							if( value > 0 ) {
								value = 1;
							}
							else if( value < 0 ) {
								value = 1;						    
							}
						}
						else {

						}
					}

					if(event->code == ABS_Y)
					{
						//printf( "nub2 y %3d\n", value );
						if( abs(value) > NUB2_CUTOFF ) {
							if( value > 0 ) {
								value = 1;
							}
							else if( value < 0 ) {
								value = 1;
							}
						}
						else {

						}
					}
					break;
			}
			break;
	}
}

int PND_OpenEventDeviceByID( int event_id )
{
	int fd;

	snprintf( event_name, sizeof(event_name), "/dev/input/event%d", event_id );
	gprintf( "Device: %s\n", event_name );
	if ((fd = open(event_name, O_RDONLY |  O_NDELAY)) < 0) {
		perror("ERROR: Could not open device");
		return 0;
	}

	if (ioctl(fd, EVIOCGVERSION, &version)) {
		perror("evtest: can't get version");
		return 0;
	}

	gprintf("Input driver version is %d.%d.%d\n",
		version >> 16, (version >> 8) & 0xff, version & 0xff);

	ioctl(fd, EVIOCGID, id);
	gprintf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
		id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);

	ioctl(fd, EVIOCGNAME(sizeof(dev_name)), dev_name);
	gprintf("Input device name: \"%s\"\n", dev_name);

	return fd;
}

int PND_OpenEventDeviceByName( char device_name[] )
{
	int fd;

	for (i = 0; 1; i++)
	{
		snprintf( event_name, sizeof(event_name), "/dev/input/event%d", i );
		gprintf( "Device: %s\n", event_name );
		if ((fd = open(event_name, O_RDONLY |  O_NDELAY)) < 0) {
			perror("ERROR: Could not open device");
			return 0;
		}
		if (fd < 0) break; /* no more devices */

		ioctl(fd, EVIOCGNAME(sizeof(dev_name)), dev_name);
		if (strcmp(dev_name, device_name) == 0)
		{
			if (ioctl(fd, EVIOCGVERSION, &version)) {
				perror("evtest: can't get version");
				return 0;
			}

			gprintf("Input driver version is %d.%d.%d\n",
				version >> 16, (version >> 8) & 0xff, version & 0xff);

			ioctl(fd, EVIOCGID, id);
			gprintf("Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
				id[ID_BUS], id[ID_VENDOR], id[ID_PRODUCT], id[ID_VERSION]);

			ioctl(fd, EVIOCGNAME(sizeof(dev_name)), dev_name);
			gprintf("Input device name: \"%s\"\n", dev_name);
		  
			return fd;
		}
		close(fd); /* we don't need this device */
	}
	return 0;
}

#endif
