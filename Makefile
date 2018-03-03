#Makefile by Kenneth Halim

SRC := src
OUT := bin
OBJ := $(OUT)/obj
EXEC := main

#Finding source file
SRC_FOLDERS := $(shell find $(SRC) -type d)
SRC_FILES := $(shell find $(SRC) -type f -name "*.cpp")
SRC_HEADERS := $(shell find $(SRC) -type f -name "*.h")

#Object file
OBJ_FOLDERS := $(addprefix $(OBJ)/,$(SRC_FOLDERS))
OBJ_FILES := $(addprefix $(OBJ)/,$(patsubst %.cpp,%.o,$(SRC_FILES)))


CC:= g++
CC_FLAG:= -Wall -I $(SRC) -std=c++11 
all: generate | $(EXEC) 

generate:
	@cd src && (sh .gen.sh > applib)
$(EXEC):$(OBJ_FILES) 
	@$(CC) $^ -o $@ -pthread -lpthread
	@echo "Linking: $@"
$(OBJ_FOLDERS):
	@echo "Make Directory : $@"
	@mkdir -p $@

$(OBJ)/%.o: %.cpp $(SRC_HEADERS) | $(OBJ_FOLDERS) 
	@$(CC)  $(CC_FLAG) -c  $<  -o $@ -lpthread
	@echo "Compiling: $<"
clean:
	@rm -rf $(OBJ)
	@echo "Removing: $(OBJ)"
	@rm -rf $(EXEC)
	@echo "Removing: $(EXEC)"

run: exec
exec:
	sudo ./main 2> main.log
