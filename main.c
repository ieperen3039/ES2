#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_util.h"
#include "network.h"
#include "mobilenetv2.h"
#include "preprocessing.h"
#include "timer.h"

int argmax(BLOB* b) {
	//find index of channel that is maximum
	float m = b->data[0];
	int i = 0;
	for (int z = 1; z < b->d; z++) {
		if (b->data[z] > m) {
			m = b->data[z];
			i = z;
		}
	}
	return i;
}

int main(int argc, char* argv[]) {
	if (argc != 2 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
		printf("Usage: %s <input.png>\n", argv[0]);
        exit(1);
	}

#ifndef SILENT
	//set stdout to line buffered such that piping to "tee" does not delay
	setvbuf(stdout, NULL, _IOLBF, 0);
#endif

	//read png into image structure
	printf("Loading image %s\n", argv[1]);
	BLOB* img = read_png(argv[1]);

	//Do preprocessing of the image
    printf("Starting preprocessing...\n");

	cpu_preprocess(img);
    printf("Preprocessing finished\n");

	//perform inference
	printf("Performing inference\n");

	//warp a timer around the network evaluation
	GET_NAMED_TIMER("Complete Network",
			//evaluate the network
			BLOB* out = network(&mobilenetv2, img);
	); //end of timer wrapper

	//get class index of maximum
	int class_idx = argmax(out);
	if (!(class_idx >= 0 && class_idx <= 999)) {
        printf("ERROR: provided class index (%d) is out of bounds!\n", class_idx);
        exit(1);
	}

	//print the class index
	printf("Detected class: %d\n", class_idx);

	//print the class label
	switch (class_idx) {
	case 281:
		printf("\033[0;33mRobot finds kitteh!\033[0m\n");
		break;
	case 285:
		printf("\033[0;33mRobot finds \033[0;35mEgyptsjun\033[0;33m kitteh - so exotic!\033[0m\n");
		break;
	case 291:
		printf("\033[0;33mRobot finds \033[0;34mKing of Kittehs\033[0;33m- much wow!\033[0m\n");
		break;
	default:
		printf("\033[1;37mIz not kitteh. Itz a \"%s\"\033[0m\n",
				labels[class_idx]);
		break;
	}

	//cleanup output
	blob_free(out);

	//cleanup input image
	blob_free(img);

	return 0;
}
