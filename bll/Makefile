include ../../lightframe/Makefile_public

HALLSERVER_DIR = $(BASE_DIR)/hallserver

HALLSERVER_INCLUDE_DIR = $(HALLSERVER_DIR)

HALLSERVER_OBJ_DIR = $(HALLSERVER_DIR)/.objs

SRC = $(wildcard *.cpp)
OBJS = $(addprefix $(HALLSERVER_OBJ_DIR)/, $(subst .cpp,.o,$(SRC)))

INC = -I$(LIGHTFRAME_INCLUDE_DIR) -I$(HALLSERVER_INCLUDE_DIR) -I$(MYSQL_INCLUDE_DIR) -I$(PUBLIC_INCLUDE_DIR)

all : $(OBJS)

$(HALLSERVER_OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS)
