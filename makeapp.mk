OBJ_PATH := obj
SRC_PATH := sources
INC_PATH := includes
OBJS := $(addprefix $(OBJ_PATH)/,$(addsuffix .o,$(basename $(SOURCES))))
LIBDEPS := $(addprefix ../,$(LIBS))
LIBINCDEPS := $(addsuffix /includes,$(LIBDEPS))
LIBDEPS := $(addsuffix /$(OBJ_PATH)/lib,$(LIBDEPS))
LIBDEPS := $(join $(LIBDEPS),$(LIBS))
LIBDEPS := $(addsuffix .a,$(LIBDEPS))
TARGET := $(OBJ_PATH)/$(PRODUCT)
INC_FLAGS := $(addprefix -I,$(INC_PATH))
LIBINC_FLAGS := $(addprefix -I,$(LIBINCDEPS))
CPPFLAGS := $(INC_FLAGS) $(LIBINC_FLAGS) -DLOGGER_MODULE=\"$(PRODUCT)\"
	
$(TARGET): $(LIBDEPS) $(OBJS)
	gcc $(OBJS) $(LIBDEPS) -o $(TARGET) -lstdc++ -lpthread
	
-include $(OBJS:.o=.d)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	mkdir -p $(OBJ_PATH)
	gcc -c -g $(CPPFLAGS) $(SRC_PATH)/$*.cpp -o $(OBJ_PATH)/$*.o
	gcc -MM -MT $(OBJ_PATH)/$*.o $(CPPFLAGS) $(SRC_PATH)/$*.cpp > $(OBJ_PATH)/$*.d

all: $(TARGET)

clean:
	rm -f $(OBJ_PATH)/*
