#include "hidblinker.h"

#include <libusb-1.0/libusb.h> 

#include <stdio.h> 
#include <stdlib.h>

#define VENDOR_ID 		(0x04D8)
#define PRODUCT_ID 		(0x003F)

#define ENDPOINT_INT_IN	(0x81)
#define ENDPOINT_INT_OUT	(0x01)

#define INTERFACE		(0)

#define TIMEOUT		(5000)

#define CMD_TOGGLE_LED          (0x80)
#define CMD_READ_BUTTON         (0x81)

struct hidblinker {
	struct libusb_context *context;
	struct libusb_device_handle *handler;	
};

static void close_libusb(struct hidblinker *dev)
{
	libusb_close(dev->handler); 
	libusb_exit(dev->context); 
}

struct hidblinker* hidblinker_init(void)
{
	struct hidblinker *dev;
	int r;

	dev = (struct hidblinker*)malloc(sizeof(struct hidblinker));
	if (dev == NULL) {
		fprintf(stderr, "Cannot allocate hidblinker object\n"); 
         	return NULL;
	}
	
	r = libusb_init(&dev->context); 
     	if (r < 0) { 
         	fprintf(stderr, "Failed to initialise libusb\n"); 
		free(dev);
         	return NULL;
	}

	dev->handler = libusb_open_device_with_vid_pid(dev->context, VENDOR_ID, PRODUCT_ID); 
     	if (dev->handler == NULL) {
		fprintf(stderr, "Cannot find hidblinker device\n"); 
		free(dev);
         	return NULL;
	}

	libusb_detach_kernel_driver(dev->handler, INTERFACE);      
 
	r = libusb_claim_interface(dev->handler, INTERFACE); 
	if (r < 0) {
         	fprintf(stderr, "LibUsb claim interface error: %d\n", r); 
		
		libusb_attach_kernel_driver(dev->handler, INTERFACE);
 		close_libusb(dev);
		free(dev);
		return NULL;
	}

     	return dev; 
}

int hidblinker_toggle_led(struct hidblinker *dev)
{
	int r;
	int size;
	char data[1];
	
	data[0] = CMD_TOGGLE_LED;
	r = libusb_interrupt_transfer(dev->handler, ENDPOINT_INT_OUT, data, 1, &size, TIMEOUT); 
     	if (r < 0) { 
         	fprintf(stderr, "Interrupt write error: %d\n", r); 
         	return r; 
	}
	return 0;
}

int hidblinker_read_button(struct hidblinker *dev, char *state)
{
	int r;
	int size;
	char data[2];
	
	data[0] = CMD_READ_BUTTON;
	r = libusb_interrupt_transfer(dev->handler, ENDPOINT_INT_OUT, data, 1, &size, TIMEOUT); 
     	if (r < 0) { 
         	fprintf(stderr, "Interrupt write error: %d\n", r); 
         	return r; 
	}

	r = libusb_interrupt_transfer(dev->handler, ENDPOINT_INT_IN, data, 2, &size, TIMEOUT); 
     	if (r < 0) { 
         	fprintf(stderr, "Interrupt read error: %d\n", r); 
         	return r; 
	}

        if ((size != 2) || (data[0] != (char)CMD_READ_BUTTON)) {
                fprintf(stderr, "Wrong response\n"); 
                return -1;
        }

        *state = data[1];
	return 0;
}

void hidblinker_close(struct hidblinker *dev)
{
	libusb_release_interface(dev->handler, INTERFACE); 
	libusb_attach_kernel_driver(dev->handler, INTERFACE);      
	close_libusb(dev);
	free(dev);
}


