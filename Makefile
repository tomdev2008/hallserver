include ../lightframe/Makefile_public

HALLSERVER_DIR = $(BASE_DIR)/hallserver

HALLSERVER_INCLUDE_DIR = $(HALLSERVER_DIR)

BLL_DIR = bll
CTL_DIR = ctl
DAL_DIR = dal
DEF_DIR = def
CONFIG_DIR = config

HALLSERVER_OBJ_DIR = $(HALLSERVER_DIR)/.objs

HALLSERVER_LDFLAGS = $(LDFLAGS) -ldl -lpthread

TARGET	= $(DEBUG_TARGET)

DEBUG_TARGET = $(BIN_DIR)/vdc_hallserver$(BIN_SUFFIX)

HALLSERVER_SRC = $(wildcard *.cpp)
DAL_SRC = $(wildcard $(DAL_DIR)/*.cpp)
DEF_SRC = $(wildcard $(DEF_DIR)/*.cpp)
CTL_SRC = $(wildcard $(CTL_DIR)/*.cpp)
BLL_SRC = $(wildcard $(BLL_DIR)/*.cpp)
CONFIG_SRC = $(wildcard $(CONFIG_DIR)/*.cpp)
TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)

HALLSERVER_OBJS = $(addprefix $(HALLSERVER_OBJ_DIR)/, $(subst .cpp,.o,$(HALLSERVER_SRC)))

OBJS = $(wildcard $(HALLSERVER_OBJ_DIR)/*.o)

INC = -I$(LIGHTFRAME_INCLUDE_DIR) -I$(HALLSERVER_INCLUDE_DIR) -I$(MYSQL_INCLUDE_DIR) -I$(PUBLIC_INCLUDE_DIR)

all : $(TARGET)

$(TARGET) : $(HALLSERVER_OBJS) COMMON LIGHTFRAME DAL DEF CTL BLL CONFIG
	$(CXX)  -o $@ $(OBJS) $(HALLSERVER_LDFLAGS)
$(HALLSERVER_OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@
COMMON:
	cd $(COMMON_DIR); make
DAL:
	cd $(DAL_DIR); make
DEF:
	cd $(DEF_DIR); make
CTL:
	cd $(CTL_DIR); make
BLL:
	cd $(BLL_DIR); make
CONFIG:
	cd $(CONFIG_DIR); make
LIGHTFRAME:
	cd $(LIGHTFRAME_DIR); make

clean: 
	cd $(COMMON_DIR); make clean;
	cd $(LIGHTFRAME_DIR); make clean;
	cd $(CONFIG_DIR); make clean;
	cd $(DAL_DIR); make clean;
	cd $(DEF_DIR); make clean;
	cd $(CTL_DIR); make clean;
	cd $(BLL_DIR); make clean;
	rm -f $(OBJS) $(TARGET)


