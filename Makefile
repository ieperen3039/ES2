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

# Location of the CUDA Toolkit binaries and libraries
CUDA_PATH       ?= /usr/local/cuda
CUDA_INC_PATH   ?= $(CUDA_PATH)/include
CUDA_BIN_PATH   ?= $(CUDA_PATH)/bin
ifneq ($(DARWIN),)
  CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib
else
  ifeq ($(OS_SIZE),32)
    CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib
  else
    CUDA_LIB_PATH  ?= $(CUDA_PATH)/lib64
  endif
endif


# Common binaries
NVCC            ?= $(shell which nvcc > /dev/null && which nvcc || echo "$(CUDA_BIN_PATH)/nvcc")
CC         =  g++

# Extra user flags
EXTRA_NVCCFLAGS ?=
EXTRA_LDFLAGS   ?=
#CFLAGS = -O3 -Wall std=gnu99
CFLAGS=-O3 -Wall

# CUDA code generation flags
#GENCODE_SM10    := -gencode arch=compute_10,code=sm_10
#GENCODE_SM20    := -gencode arch=compute_20,code=sm_20
GENCODE_SM30    := -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35
GENCODE_FLAGS   := $(GENCODE_SM30)


# OS-specific build flagi
ifneq ($(DARWIN),)
      ifneq ($(CU_SRCS),)
          LDFLAGS   := -Xlinker -rpath $(CUDA_LIB_PATH) -L$(CUDA_LIB_PATH) -lcudart
      endif
      CCFLAGS   := -arch $(OS_ARCH)
else
  ifeq ($(OS_SIZE),32)
      ifneq ($(CU_SRCS),)
          LDFLAGS   := -L$(CUDA_LIB_PATH) -lcudart
      endif
      CCFLAGS   := -m32
  else
      ifneq ($(CU_SRCS),)
          LDFLAGS   := -L$(CUDA_LIB_PATH) -lcudart
      endif
      CCFLAGS   := -m64
  endif
endif

# OS-architecture specific flags
ifeq ($(OS_SIZE),32)
      NVCCFLAGS := -m32
else
      NVCCFLAGS := -m64
endif

# Common includes and paths for CUDA
INCLUDES      := -I$(CUDA_INC_PATH) -I. -I.. -I../../common/inc

#openCL libs
CL_LIBS=OpenCL


SRCS=$(wildcard *.c)
CPP_SRCS=$(wildcard *.cpp)
CU_SRCS=$(wildcard *.cu)
OBJS=$(SRCS:.c=.o) $(CPP_SRCS:.cpp=.o)  $(CU_SRCS:.cu=.o)
EXE=mobilenetv2

#Target Rules

default:check

include images.mk

.PHONY:check
check: converted_$(basename $(notdir $(word 1,$(IMAGE_URLS)))).png $(EXE)
	./$(EXE) $< | tee $@
	@grep -q "Detected class: $(strip $(word 1, $(CLASS_IDX)))" $@ && printf "$(GREEN)correctly identified iimage $<$(NC)\n"

.PRECIOUS:$(EXE)
$(EXE):$(OBJS)
	$(CC) $(INCLUDES) $(OBJS) -L $(CUDA_LIB_PATH) -framework $(CL_LIBS) -lpng $(LDFLAGS) $(EXTRA_LDFLAGS) -o $(EXE)

%.o:%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

%.o:%.cu
	$(NVCC) $(NVCCFLAGS) $(EXTRA_NVCCFLAGS) $(GENCODE_FLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS) $(CU_OBJS) $(EXE)

#Create an archive of the code
USER=$(shell whoami)
ZIPFILE=$(USER).zip
zip:$(ZIPFILE)
$(ZIPFILE):clean
	zip -r $@ ./* -x ./bins/*

#build dependency files if we are not cleaning
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

