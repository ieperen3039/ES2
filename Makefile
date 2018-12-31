# OS Name (Linux or Darwin)
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

# Flags to detect 32-bit or 64-bit OS platform
OS_SIZE = $(shell uname -m | sed -e "s/i.86/32/" -e "s/x86_64/64/")
OS_ARCH = $(shell uname -m | sed -e "s/i386/i686/")

# These flags will override any settings
ifeq ($(i386),1)
  OS_SIZE = 32
  OS_ARCH = i686
endif

ifeq ($(x86_64),1)
  OS_SIZE = 64
  OS_ARCH = x86_64
endif

# Flags to detect either a Linux system (linux) or Mac OSX (darwin)
DARWIN = $(strip $(findstring DARWIN, $(OSUPPER)))

# Common binaries
CC         =  g++

# Extra user flags
#CFLAGS = -O3 -Wall std=gnu99
CFLAGS=-O3 -Wall

# Pass define flags
ifdef DEBUG
CFLAGS += -DDEBUG -g
endif
ifdef SILENT
CFLAGS += -DSILENT
endif
ifdef CPU
CFLAGS += -DCPU_ONLY
endif
ifdef TIMING
CFLAGS += -DTIMING
endif

# OS-specific build flags
ifneq ($(DARWIN),)
      CCFLAGS   := -arch $(OS_ARCH)
else
  ifeq ($(OS_SIZE),32)
      CCFLAGS   := -m32
  else
      CCFLAGS   := -m64
  endif
endif

# Common include paths
INCLUDES      :=  -I.

# OpenCL libs
CL_LIBS=OpenCL

SRCS=$(wildcard *.c)
CPP_SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)  $(CU_SRCS:.cu=.o)
EXE=mobilenetv2

# Target Rules

default:check

include images.mk

.PHONY:check
check: converted_$(basename $(notdir $(word 1,$(IMAGE_URLS)))).png $(EXE)
	./$(EXE) $< | tee $@
	@grep -q "Detected class: $(strip $(word 1, $(CLASS_IDX)))" $@ && printf "$(GREEN)correctly identified image $<$(NC)\n"

.PRECIOUS:$(EXE)
$(EXE):$(OBJS)
	$(CC) $(OBJS) $(INCLUDES) -l$(CL_LIBS) -lpng -o $(EXE)

%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

#Create an archive of the code
USER=$(shell whoami)
ZIPFILE=$(USER).zip
zip:$(ZIPFILE)
$(ZIPFILE):clean
	zip -r $@ ./* -x ./bins/*

# Build dependency files if we are not cleaning
ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEPS)
endif

CLEAN=$(OBJS) $(DEPS) $(EXE) check $(ZIPFILE)

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
	rm -rf $(OBJS) $(EXE)
