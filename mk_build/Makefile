#Directories
LIB_LOCAL_DIR := ../../../liblocal
LIB_FLTK_DIR := $(LIB_LOCAL_DIR)/libfltk_gcc
LIB_JSONCPP_DIR := $(LIB_LOCAL_DIR)/libjson_gcc
SRC_DIR = ../src
OBJ_DIR := obj
LIB_DIR := lib

#Includes
INC_FLTK := $(LIB_FLTK_DIR)/include
INC_JSON := $(LIB_JSONCPP_DIR)
INC_SYS := $(LIB_LOCAL_DIR)

#Compiler Exceptions
INC_SYS_FLAGS := -isystem $(INC_SYS) -isystem $(INC_FLTK) -isystem $(INC_JSON)

#Compiled Objects
OBJ := $(OBJ_DIR)/jsoncpp.o $(OBJ_DIR)/gautier_visual_model.o $(OBJ_DIR)/gautier_visual_application_window.o $(OBJ_DIR)/gautier_program.o $(OBJ_DIR)/gautier.o

#Libraries
LIB_FLTK := $(LIB_FLTK_DIR)/lib/libfltk.a

#Compiler parameters. Minimum standard.
CPP_STD := -std=c++11
OPTO_PARAM := -O3 #full general optimization. Use this for released versions.
#OPTO_PARAM := -Os #optimize to a small program size. Useful for comparison.
#OPTO_PARAM := -g #full debugging. This should be default for testing and revision.
CPP_MIN := $(OPTO_PARAM) $(CPP_STD) -Wpedantic -Wno-system-headers 
CPP_DBG := -fabi-version=0 -fcheck-new -fconstexpr-depth=512 -ffor-scope -fstrict-aliasing -fstrict-overflow -fvisibility-inlines-hidden -Wabi -Wall -Wcast-align -Wcast-qual -Wconversion -Wconversion-null -Wctor-dtor-privacy -Wdelete-non-virtual-dtor -Wdouble-promotion -Weffc++ -Wextra -Wfloat-equal -Wformat=2 -Wliteral-suffix -Wnarrowing -Wnoexcept -Wno-non-template-friend -Wno-unused-function -Wnon-virtual-dtor -Wno-pmf-conversions -Wold-style-cast -Woverloaded-virtual -Wreorder -Wshadow -Wsign-promo -Wstrict-aliasing=3 -Wstrict-null-sentinel -Wstrict-overflow=5 -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wuseless-cast 

#Builders
CPP_COMPILE := $(CPP_MIN) -c $(CPP_DBG) 
CPP_LINK := $(CXX) $(CPP_STD) $(OPTO_PARAM) 

#Linker parameters
LIB_LINK := $(LIB_FLTK) `$(LIB_FLTK_DIR)/bin/fltk-config --ldstaticflags`

#---------------------------------------------------------------------------------------------START

#Build the program
gautier : $(OBJ)
	$(CPP_LINK) -L$(LIB_FLTK_DIR)/lib -o $@ $(OBJ) $(LIB_LINK)

#at this stage in the project, all should compile if any header change
#beyond that, object files should change if its implementation changes.

#Build the object files
$(OBJ_DIR)/gautier.o: $(SRC_DIR)/main.cxx \
 $(SRC_DIR)/gautier_program.hxx \
 $(SRC_DIR)/gautier_visual_model.hxx \
 $(SRC_DIR)/gautier_visual_application_window.hxx 
	$(CXX) $(CPP_COMPILE) -I$(INC_FLTK) $(INC_SYS_FLAGS) -o $@ $< 

$(OBJ_DIR)/gautier_program.o: $(SRC_DIR)/gautier_program.cxx \
 $(SRC_DIR)/gautier_program.hxx \
 $(SRC_DIR)/gautier_visual_model.hxx \
 $(SRC_DIR)/gautier_visual_application_window.hxx 
	$(CXX) $(CPP_COMPILE) -I$(INC_FLTK) -I$(INC_JSON) $(INC_SYS_FLAGS) -o $@ $< 

$(OBJ_DIR)/gautier_visual_application_window.o: $(SRC_DIR)/gautier_visual_application_window.cxx \
 $(SRC_DIR)/gautier_program.hxx \
 $(SRC_DIR)/gautier_visual_model.hxx \
 $(SRC_DIR)/gautier_visual_application_window.hxx 
	$(CXX) $(CPP_COMPILE) -I$(INC_FLTK) $(INC_SYS_FLAGS) -o $@ $< 

$(OBJ_DIR)/gautier_visual_model.o: $(SRC_DIR)/gautier_visual_model.cxx \
 $(SRC_DIR)/gautier_program.hxx \
 $(SRC_DIR)/gautier_visual_model.hxx \
 $(SRC_DIR)/gautier_visual_application_window.hxx 
	$(CXX) $(CPP_COMPILE) -I$(INC_FLTK) $(INC_SYS_FLAGS) -o $@ $< 

$(OBJ_DIR)/jsoncpp.o : infrastructure_build
	cp --update $(LIB_JSONCPP_DIR)/jsoncpp.o $(OBJ_DIR)/
	touch ../src/main.cxx

#Setup prerequisite directories and files.
infrastructure_build: $(LIB_FLTK) $(LIB_JSONCPP_DIR)/jsoncpp.o
	mkdir -p $(OBJ_DIR)

.PHONY : clean

clean : 
	rm -r obj/
	rm gautier_*
	rm *~

touch :
	touch ../src/*.hxx
	touch ../src/main.cxx

#---------------------------------------------------------------------------------------------END

#Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0 . Software distributed under the License is distributed on an "AS IS" BASIS, NO WARRANTIES OR CONDITIONS OF ANY KIND, explicit or implicit. See the License for details on permissions and limitations.

