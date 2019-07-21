mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS += balancer.o balancer_param.o

APPL_CXXOBJS += \
	LineMonitor.o \
	TailController.o \
	BalancerCpp.o \
	Measurer.o \
	InvertedWalker.o \
	LineTracer.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += $(mkfile_path)app $(mkfile_path)unit
APPL_DIR += $(mkfile_path)app $(mkfile_path)measurement

INCLUDES += \
	-I$(mkfile_path)app \
	-I$(mkfile_path)measurement \
	-I$(mkfile_path)unit
