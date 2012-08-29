#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/image.o \
	${OBJECTDIR}/feature.o \
	${OBJECTDIR}/tools.o \
	${OBJECTDIR}/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lm `pkg-config --libs cunit` `pkg-config --libs opencv`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/features

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/features: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/features ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/image.o: image.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/image.o image.c

${OBJECTDIR}/feature.o: feature.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/feature.o feature.c

${OBJECTDIR}/tools.o: tools.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/tools.o tools.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/featureUnitTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS} -lcunit 

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/imageUnitTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS} -lcunit 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/toolsUnitTest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} -lcunit 


${TESTDIR}/tests/featureUnitTest.o: tests/featureUnitTest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -MMD -MP -MF $@.d -o ${TESTDIR}/tests/featureUnitTest.o tests/featureUnitTest.c


${TESTDIR}/tests/imageUnitTest.o: tests/imageUnitTest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -MMD -MP -MF $@.d -o ${TESTDIR}/tests/imageUnitTest.o tests/imageUnitTest.c


${TESTDIR}/tests/toolsUnitTest.o: tests/toolsUnitTest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -MMD -MP -MF $@.d -o ${TESTDIR}/tests/toolsUnitTest.o tests/toolsUnitTest.c


${OBJECTDIR}/image_nomain.o: ${OBJECTDIR}/image.o image.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/image.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/image_nomain.o image.c;\
	else  \
	    ${CP} ${OBJECTDIR}/image.o ${OBJECTDIR}/image_nomain.o;\
	fi

${OBJECTDIR}/feature_nomain.o: ${OBJECTDIR}/feature.o feature.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/feature.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/feature_nomain.o feature.c;\
	else  \
	    ${CP} ${OBJECTDIR}/feature.o ${OBJECTDIR}/feature_nomain.o;\
	fi

${OBJECTDIR}/tools_nomain.o: ${OBJECTDIR}/tools.o tools.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/tools.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/tools_nomain.o tools.c;\
	else  \
	    ${CP} ${OBJECTDIR}/tools.o ${OBJECTDIR}/tools_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.c 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g `pkg-config --cflags cunit` `pkg-config --cflags opencv`    -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/main_nomain.o main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/features

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
