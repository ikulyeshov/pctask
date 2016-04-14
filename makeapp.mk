OBJ_PATH := obj
SRC_PATH := sources
INC_PATH := includes api
OBJS := $(addprefix $(OBJ_PATH)/,$(addsuffix .o,$(basename $(SOURCES))))
LIBDEPS := $(addprefix ../,$(LIBS))
LIBINCDEPS := $(addsuffix /api,$(LIBDEPS))
LIBDEPS := $(addsuffix /$(OBJ_PATH)/lib,$(LIBDEPS))
LIBDEPS := $(join $(LIBDEPS),$(LIBS))
LIBDEPS := $(addsuffix .a,$(LIBDEPS))
TARGET := $(OBJ_PATH)/$(PRODUCT).exe
INC_FLAGS := $(addprefix -I,$(INC_PATH))
LIBINC_FLAGS := $(addprefix -I,$(LIBINCDEPS))
CPPFLAGS := -lstdc++ $(INC_FLAGS) $(LIBINC_FLAGS)
	
$(TARGET): $(LIBDEPS) $(OBJS)
	gcc $(OBJS) $(LIBDEPS) -o $(TARGET)
	
-include $(OBJS:.o=.d)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	mkdir -p $(OBJ_PATH)
	gcc -c $(CPPFLAGS) $(SRC_PATH)/$*.cpp -o $(OBJ_PATH)/$*.o
	gcc -MM -MT $(OBJ_PATH)/$*.o $(CPPFLAGS) $(SRC_PATH)/$*.cpp > $(OBJ_PATH)/$*.d
#	@cp -f $(OBJ_PATH)/$*.d $(OBJ_PATH)/$*.d.tmp
#	@sed -e 's/.*://' -e 's/\\$$//' < $(OBJ_PATH)/$*.d.tmp | fmt -1 | \
#	 sed -e 's/^ *//' -e 's/$$/:/' >> $(OBJ_PATH)/$*.d
#	@rm -f $(OBJ_PATH)/$*.d.tmp

all: $(TARGET)

clean:
	rm -f $(OBJ_PATH)/*
