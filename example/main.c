 
#include <hidblinker.h>

#include <stdio.h>
#include <stdlib.h>
  
static struct hidblinker *dev;
 
int main(void) 
{
	int r;
	char state;
	int size;

	dev = hidblinker_init();
	if (dev == NULL) {
		exit(1);
	}

	r = hidblinker_toggle_led(dev);
	if (r < 0) {
		hidblinker_close(dev);
		exit(1);		
	}

	r = hidblinker_read_button(dev, &state);
	if (r < 0) {
		hidblinker_close(dev);
		exit(1);		
	}

	printf("Button: %d\n", state);

	hidblinker_close(dev);
	
	return 0;
}

