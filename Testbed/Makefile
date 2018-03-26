#
# OMNeT++/OMNEST Makefile for Testbed
#
# This file was generated with the command:
#  opp_makemake -f --deep -lmysqlcppconn
#

# Name of target to be created (-o option)
TARGET = Testbed$(D)$(EXE_SUFFIX)
TARGET_DIR = .

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH =\
	-I. \
    -Iconfigurations \
    -Iconfigurations/ACARCConfigs \
    -Iconfigurations/categorial \
    -Iconfigurations/Hybrid \
    -Iconfigurations/Infinity \
    -Iconfigurations/Special \
    -Iconfigurations/Topologies \
    -Iresults \
    -Isrc \
    -Isrc/cache \
    -Isrc/cache/admissionStretegies \
    -Isrc/cache/admissionStretegies/lcd \
    -Isrc/cache/admissionStretegies/lce \
    -Isrc/cache/admissionStretegies/mcd \
    -Isrc/cache/admissionStretegies/nhit \
    -Isrc/cache/admissionStretegies/prob \
    -Isrc/cache/admissionStretegies/rand \
    -Isrc/cache/evictionStrategies \
    -Isrc/cache/evictionStrategies/acdc \
    -Isrc/cache/evictionStrategies/arc1 \
    -Isrc/cache/evictionStrategies/arc2 \
    -Isrc/cache/evictionStrategies/fifo \
    -Isrc/cache/evictionStrategies/ghostLists \
    -Isrc/cache/evictionStrategies/infinity \
    -Isrc/cache/evictionStrategies/lfu \
    -Isrc/cache/evictionStrategies/lfuda \
    -Isrc/cache/evictionStrategies/lru \
    -Isrc/cache/evictionStrategies/nodes \
    -Isrc/cache/evictionStrategies/probationary \
    -Isrc/cache/evictionStrategies/rand \
    -Isrc/cache/evictionStrategies/segment \
    -Isrc/cache/evictionStrategies/slru \
    -Isrc/cache/evictionStrategies/ttl \
    -Isrc/database \
    -Isrc/simulation

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =  -lmysqlcppconn

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/src/cache/BaseCache.o \
    $O/src/cache/CacheFactory.o \
    $O/src/cache/admissionStretegies/AdmissionStrategyFactory.o \
    $O/src/cache/admissionStretegies/BasicAdmissionStrategy.o \
    $O/src/cache/admissionStretegies/lcd/LCD.o \
    $O/src/cache/admissionStretegies/lce/LCE.o \
    $O/src/cache/admissionStretegies/mcd/MCD.o \
    $O/src/cache/admissionStretegies/nhit/NHIT.o \
    $O/src/cache/admissionStretegies/prob/PROB.o \
    $O/src/cache/admissionStretegies/rand/RandomAdmission.o \
    $O/src/cache/evictionStrategies/BasicEvictionStrategy.o \
    $O/src/cache/evictionStrategies/CacheSegmentFactory.o \
    $O/src/cache/evictionStrategies/EvictionStrategyFactory.o \
    $O/src/cache/evictionStrategies/SLRUProbationaryCacheFactory.o \
    $O/src/cache/evictionStrategies/acdc/ACDCCache.o \
    $O/src/cache/evictionStrategies/acdc/BasicSSAS.o \
    $O/src/cache/evictionStrategies/acdc/LargestGhostListSSAS.o \
    $O/src/cache/evictionStrategies/acdc/LeftFirstSSAS.o \
    $O/src/cache/evictionStrategies/acdc/ProbationaryFirstSSAS.o \
    $O/src/cache/evictionStrategies/acdc/RelativeLargestGhostListSSAS.o \
    $O/src/cache/evictionStrategies/acdc/RelativeSmallestGhostListSSAS.o \
    $O/src/cache/evictionStrategies/acdc/RightFirstSSAS.o \
    $O/src/cache/evictionStrategies/acdc/SmallestGhostListSSAS.o \
    $O/src/cache/evictionStrategies/acdc/SSASFactory.o \
    $O/src/cache/evictionStrategies/arc1/ARC1Cache.o \
    $O/src/cache/evictionStrategies/arc2/ARC2Cache.o \
    $O/src/cache/evictionStrategies/fifo/FIFOCache.o \
    $O/src/cache/evictionStrategies/ghostLists/BasicGhostList.o \
    $O/src/cache/evictionStrategies/ghostLists/GhostList.o \
    $O/src/cache/evictionStrategies/ghostLists/LifeTimeGhostList.o \
    $O/src/cache/evictionStrategies/ghostLists/LimitedGhostList.o \
    $O/src/cache/evictionStrategies/infinity/InfinityCache.o \
    $O/src/cache/evictionStrategies/lfu/LFUCache.o \
    $O/src/cache/evictionStrategies/lfuda/LFUDACache.o \
    $O/src/cache/evictionStrategies/lru/LRUCache.o \
    $O/src/cache/evictionStrategies/nodes/FrequencyNode.o \
    $O/src/cache/evictionStrategies/nodes/RecencyNode.o \
    $O/src/cache/evictionStrategies/probationary/ARCProbationaryCache.o \
    $O/src/cache/evictionStrategies/probationary/BasicProbationaryCache.o \
    $O/src/cache/evictionStrategies/probationary/SLRUProbationaryCache.o \
    $O/src/cache/evictionStrategies/rand/RANDCache.o \
    $O/src/cache/evictionStrategies/segment/BasicCacheSegment.o \
    $O/src/cache/evictionStrategies/segment/FIFOCacheSegment.o \
    $O/src/cache/evictionStrategies/segment/LFUCacheSegment.o \
    $O/src/cache/evictionStrategies/segment/LRUCacheSegment.o \
    $O/src/cache/evictionStrategies/segment/RANDCacheSegment.o \
    $O/src/cache/evictionStrategies/segment/TTLCacheSegment.o \
    $O/src/cache/evictionStrategies/slru/SLRUCache.o \
    $O/src/cache/evictionStrategies/ttl/TTLCache.o \
    $O/src/database/AllClientsRequestCursor.o \
    $O/src/database/ClientRequestCursor.o \
    $O/src/database/CursorFactory.o \
    $O/src/database/DatabaseFactory.o \
    $O/src/database/DBConnection.o \
    $O/src/database/DBCursor.o \
    $O/src/database/FirstTimestampRequestCursor.o \
    $O/src/database/MetaRequestCursor.o \
    $O/src/database/MySqlConnection.o \
    $O/src/simulation/Client.o \
    $O/src/simulation/ClientCreator.o \
    $O/src/simulation/Hub.o \
    $O/src/simulation/NetworkInformation.o \
    $O/src/simulation/OriginServer.o \
    $O/src/simulation/RequestHandler.o \
    $O/src/simulation/ReverseProxy.o \
    $O/src/simulation/ReverseProxyCreator.o \
    $O/src/simulation/SegmentRequest_m.o \
    $O/src/simulation/VideoSegment_m.o

# Message files
MSGFILES = \
    src/simulation/SegmentRequest.msg \
    src/simulation/VideoSegment.msg

# SM files
SMFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) $(IMPORT_DEFINES)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET_DIR)/$(TARGET)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clangc2)
	$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET)
	$(Q)-rm -f $(TARGET_DIR)/$(TARGET:%.dll=%.lib)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(MAKE) -s clean MODE=release
	$(Q)$(MAKE) -s clean MODE=debug
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

# include all dependencies
-include $(OBJS:%.o=%.d)
