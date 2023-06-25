LIB_DIR = $(MAIN_PATH)/Libraries

CMSIS_DIR = $(LIB_DIR)/CMSIS
CMSIS_DEVICE_DIR = $(CMSIS_DIR)/Device/ARM/ARMCM0

# define source directory
SRC		+= $(MAIN_PATH)
SRC  	+= $(CMSIS_DIR)
SRC 	+= $(CMSIS_DEVICE_DIR)/Source

# define include directory
INCLUDE	+= $(MAIN_PATH)
INCLUDE	+= $(CMSIS_DIR)/Include
INCLUDE	+= $(CMSIS_DEVICE_DIR)/Include

# define lib directory
LIB		+=

