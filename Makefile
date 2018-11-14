CC=gcc
CFLAGS= -Wall -std=gnu99
LDFLAGS= -lpng -lm

#pass some flags through to gcc when defined
ifdef DEBUG
	CFLAGS+= -DDEBUG
endif
ifdef SILENT
	CFLAGS+= -DSILENT
endif

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
EXE=mobilenetv2


#URLS of images to test
IMAGE_URLS= 	http://farm1.static.flickr.com/222/447846619_ea0e3c9609.jpg \
	        	http://farm3.static.flickr.com/2379/2087393988_be21769f95.jpg \
				http://farm1.static.flickr.com/30/59838354_052062bf8c.jpg \
				http://farm1.static.flickr.com/201/456440842_a39b783d7b.jpg \
				http://farm1.static.flickr.com/180/421280328_317f6103ec.jpg \
				http://farm3.static.flickr.com/2257/2165764491_b7efd35ab1.jpg \
				http://farm1.static.flickr.com/177/367989583_10c1f2fc38.jpg \
				http://farm1.static.flickr.com/52/139935656_50bc16c15b.jpg

#Classification index of test images (in order)
CLASS_IDX=  	281\
				281\
				285\
				291\
				728\
				279\
				285\
				281

# Getters
JOINED   = $(join $(addsuffix @,$(IMAGE_URLS)),$(CLASS_IDX))
GET_URL  = $(word 1,$(subst @, ,$1))
GET_IDX  = $(word 2,$(subst @, ,$1))

#Bash coloring
RED=\033[0;31m
GREEN=\033[0;32m
NC=\033[0m

#$1=URL $2=NAME $3=CONVERTED_PNG $4=CHECK $5=IDX
define IMAGE_BUILD_RULES

#download image
$2:
	wget "$(strip $1)" -O $2

#convert
$3:$2
	convert $2 -resize 224x224! $3

#check if correct class is identified. If not error
$4:$3 $(EXE)
	@echo "Evaluating image $3"
	./$(EXE) $3 | tee $4
	@grep -q "Detect class: $(strip $5)" $4 && echo -e "$(GREEN)correctly identified image $2$(NC)" ||  (echo -e "$(RED)Did not correctly identify image $2$(NC)"; rm -f $4; exit -1)

endef

.PHONY:check
check:converted_$(basename $(notdir $(word 1,$(IMAGE_URLS)))).png $(EXE)
	./$(EXE) $< | tee $@
	@grep -q "Detect class: $(strip $(word 1, $(CLASS_IDX)))" $@ && echo -e "$(GREEN)correctly identified image $<$(NC)" ||  echo -e "$(RED)Did not correctly identify image $<$(NC)"

#check if all images are classified correctly
check_all: $(foreach URL, $(IMAGE_URLS), check_$(basename $(notdir $(URL))))
	@echo -e "$(GREEN)All correct!$(NC)"

#define build rules for all images
$(foreach j,$(JOINED),$(eval $(call IMAGE_BUILD_RULES,\
	$(call GET_URL, $j),\
	$(notdir $(call GET_URL, $j)),\
	converted_$(basename $(notdir $(call GET_URL, $j))).png,\
	check_$(basename $(notdir $(call GET_URL, $j))),\
	$(call GET_IDX,$j)\
)))

#link the executable
.PRECIOUS:$(EXE)
$(EXE):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

#compile c files
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

#generate dependency files
%.d:%.c
	$(CC) $(CFLAGS) -MM $^ -MF $@;


#build dependency files if we are not cleaning
ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEPS)
endif

CLEAN=$(OBJS) $(DEPS) $(EXE) check

#downloaded images
CLEAN+=$(foreach URL, $(IMAGE_URLS), $(notdir $(URL)))

#converted images
CLEAN+=$(foreach URL, $(IMAGE_URLS), converted_$(basename $(notdir $(URL))).png)

#check files
CLEAN+=$(foreach URL, $(IMAGE_URLS), check_$(basename $(notdir $(URL))))


ifdef DEBUG
#add extra cleanup when debug is set (to clean up dumped blobs)
CLEAN+= *.txt *.bin
endif

clean:
	@rm -f $(CLEAN)
