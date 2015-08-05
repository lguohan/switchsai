ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

libswitchsai_la_CFLAGS = -I$(srcdir)/submodules/ocpsai/sai/inc -I$(srcdir)/../../modules/BMI/module/inc
libswitchsai_la_CFLAGS += -I$(srcdir)/src/gen-cpp
libswitchsai_la_CXXFLAGS = $(libswitchsai_la_CFLAGS)
libswitchsai_a_CFLAGS = $(libswitchsai_la_CFLAGS)
libswitchsai_a_CXXFLAGS = $(libswitchsai_a_CFLAGS)

BUILT_SOURCES = \
src/gen-cpp/switch_sai_constants.cpp \
src/gen-cpp/switch_sai_constants.h \
src/gen-cpp/switch_sai_rpc.cpp \
src/gen-cpp/switch_sai_rpc.h \
src/gen-cpp/switch_sai_types.cpp \
src/gen-cpp/switch_sai_types.h

$(BUILT_SOURCES): src/switch_sai.thrift
	$(THRIFT) -o $(srcdir)/src/ --gen cpp -r $(srcdir)/src/switch_sai.thrift

lib_LIBRARIES = libswitchsai.a

libswitchsai_la_SOURCES = \
$(BUILT_SOURCES) \
src/saiacl.c \
src/saiapi.h \
src/sai.c \
src/saifdb.c \
src/saihostintf.c \
src/saiinternal.h \
src/sailag.c \
src/saineighbor.c \
src/sainexthop.c \
src/sainexthopgroup.c \
src/saiport.c \
src/sairoute.c \
src/sairouter.c \
src/sairouterintf.c \
src/saistp.c \
src/saiswitch.c \
src/saivlan.c \
src/switch_sai_rpc_server.cpp

libswitchsai_a_SOURCES = $(libswitchsai_la_SOURCES)
