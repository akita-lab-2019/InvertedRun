mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_CXXOBJS += \
	RobotInfo.o \
	ParmAdministrator.o \
	Recorder.o \
	LineMonitor.o \
	TailController.o \
	BalancerCpp.o \
	Odometer.o \
	TailWalker.o \
	PID.o \
	LogManager.o \
	Section.o \
	SectionTracer.o \
	BluetoothManager.o \
	LineTracer.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_DIR += $(mkfile_path)log $(mkfile_path)SectionManager $(mkfile_path)include $(mkfile_path)RobotInfo

INCLUDES += \
	-I$(mkfile_path)RobotInfo\
	-I$(mkfile_path)SectionManager\
	-I$(mkfile_path)log\
	-I$(mkfile_path)include
