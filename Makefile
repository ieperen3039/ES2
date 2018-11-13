CC=gcc
CFLAGS= -Wall -std=gnu99
LDFLAGS= -lpng -lm

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
EXE=mobilenetv2


RAW_IMAGE=koala.jpeg
PROCESSED_IMAGE=scaled_$(RAW_IMAGE:.jpeg=.png)

.PHONY:$(EXE)
run:$(EXE) $(PROCESSED_IMAGE)
	./$(EXE) $(PROCESSED_IMAGE)

$(EXE):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@


#download input image
$(RAW_IMAGE):
	wget "http://farm1.static.flickr.com/159/403176078_a2415ddf33.jpg" -O $@

#scale the input image
$(PROCESSED_IMAGE):$(RAW_IMAGE)
	convert $< -resize 224x224! $@

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF $@;

include .depend

clean:
	@rm -f $(OBJS) $(EXE) $(RAW_IMAGE) $(PROCESSED_IMAGE) .depend
