# Copyright 2013-present Barefoot Networks, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

###############################################################################
#
# This file compiles switchsai.a and switchsai_thrift.a
#
###############################################################################
THIS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifndef P4FACTORY
  $(error P4FACTORY not defined)
endif
MAKEFILES_DIR := $(P4FACTORY)/makefiles

ifndef SUBMODULE_SWITCHSAI
  SUBMODULE_SWITCHSAI := ${THIS_DIR}/../..
endif

# This variable is set to ${BUILD_DIR}/lib/switchsai.a when called from p4factory.
ifndef SWITCHSAI_LIB
  $(error Compiled archive filename not defined in SWITCHSAI_LIB)
endif

# This variable is set to ${BUILD_DIR}/lib/switchsai_thrift.a when called from p4factory.
ifndef SWITCHSAI_THRIFT_LIB
  $(error Compiled thrift archive filename not defined in SWITCHSAI_THRIFT_LIB)
endif

ifndef BUILD_DIR
  $(error Build directory not defined in BUILD_DIR)
endif

SWITCHSAI_SRC_DIR := $(SUBMODULE_SWITCHSAI)/src
SWITCHSAI_INC_DIR := $(SUBMODULE_SWITCHSAI)/submodules/ocpsai/sai/inc
SWITCHSAI_SOURCES_C := $(wildcard $(SWITCHSAI_SRC_DIR)/*.c)
SWITCHSAI_SOURCES_CPP := $(wildcard $(SWITCHSAI_SRC_DIR)/*.cpp)

GLOBAL_INCLUDES := $(GLOBAL_INCLUDES) -I $(SWITCHSAI_INC_DIR)/

SWITCHSAI_BUILD_DIR := $(BUILD_DIR)/switchsai
SWITCHSAI_BUILD_SRC_DIR := $(SWITCHSAI_BUILD_DIR)/src
SWITCHSAI_OBJ_DIR := $(SWITCHSAI_BUILD_DIR)/obj
MAKE_DIR := $(SWITCHSAI_BUILD_DIR)
include $(MAKEFILES_DIR)/makedir.mk

MAKE_DIR := $(SWITCHSAI_BUILD_SRC_DIR)
include $(MAKEFILES_DIR)/makedir.mk

MAKE_DIR := $(SWITCHSAI_OBJ_DIR)
include $(MAKEFILES_DIR)/makedir.mk

MAKE_DIR := $(dir ${SWITCHSAI_THRIFT_LIB})
include ${MAKEFILES_DIR}/makedir.mk

# Rules for generating thrift files.
SWITCHSAI_THRIFT_OUTPUT_BASENAMES := switch_sai_constants switch_sai_rpc switch_sai_types
SWITCHSAI_THRIFT_OUTPUT_BASENAMES_CPP := $(addsuffix .cpp, $(SWITCHSAI_THRIFT_OUTPUT_BASENAMES))
SWITCHSAI_THRIFT_OUTPUT_BASENAMES_H := $(addsuffix .h, $(SWITCHSAI_THRIFT_OUTPUT_BASENAMES))
SWITCHSAI_THRIFT_OUTPUT_CPP := $(addprefix ${SWITCHSAI_BUILD_SRC_DIR}/, $(SWITCHSAI_THRIFT_OUTPUT_BASENAMES_CPP))
SWITCHSAI_THRIFT_OUTPUT_H := $(addprefix ${SWITCHSAI_BUILD_SRC_DIR}/, $(SWITCHSAI_THRIFT_OUTPUT_BASENAMES_H))
${SWITCHSAI_THRIFT_OUTPUT_CPP} ${SWITCHSAI_THRIFT_OUTPUT_H} : ${SWITCHSAI_SRC_DIR}/switch_sai.thrift
	thrift --gen cpp --out ${SWITCHSAI_BUILD_SRC_DIR} $<

ifdef COVERAGE
COVERAGE_FLAGS := --coverage
endif

SWITCHSAI_THRIFT_OBJS := $(addprefix $(SWITCHSAI_OBJ_DIR)/, $(addsuffix .o, $(SWITCHSAI_THRIFT_OUTPUT_BASENAMES)))
$(SWITCHSAI_THRIFT_OBJS) : $(SWITCHSAI_OBJ_DIR)/%.o : $(SWITCHSAI_BUILD_SRC_DIR)/%.cpp
	$(VERBOSE)g++ -o $@ $(COVERAGE_FLAGS) $(DEBUG_FLAGS) $(GLOBAL_INCLUDES) -I . -std=c++11 -MD -c $<

THRIFT_INPUT_FILES := ${SWITCHSAI_SRC_DIR}/switch_sai.thrift
THRIFT_DEP_FILES := ${SWITCHSAI_SRC_DIR}/switch_sai.thrift
THRIFT_SERVICE_NAMES := switch_sai_rpc
include ${MAKEFILES_DIR}/thrift-py.mk

$(SWITCHSAI_THRIFT_LIB) : ${SWITCHSAI_THRIFT_OBJS}
	$(VERBOSE)ar -rc $@ $(SWITCHSAI_THRIFT_OBJS)

MODULE := switchsai
MODULE_LIB := $(SWITCHSAI_LIB)
$(MODULE_LIB) : MODULE_INFO := switchsai
switchsai_DIR := $(SUBMODULE_SWITCHSAI)
$(MODULE)_PREREQ := $(SWITCHSAI_THRIFT_LIB)
include $(MAKEFILES_DIR)/module.mk
$(MODULE_LIB) : MODULE_INCLUDES += -I $(SWITCHSAI_BUILD_SRC_DIR)
