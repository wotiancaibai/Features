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
	${OBJECTDIR}/sift.o \
	${OBJECTDIR}/image.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/matrixUtils.o \
	${OBJECTDIR}/discretisation.o \
	${OBJECTDIR}/computeEdges.o \
	${OBJECTDIR}/spmtimesd.o \
	${OBJECTDIR}/sparsifyc.o \
	${OBJECTDIR}/feature.o \
	${OBJECTDIR}/ncutW.o \
	${OBJECTDIR}/utils.o \
	${OBJECTDIR}/tools.o \
	${OBJECTDIR}/computeW.o \
	${OBJECTDIR}/imgfeatures.o \
	${OBJECTDIR}/ICgraph.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++0x
CXXFLAGS=-std=c++0x

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lm `pkg-config --libs opencv` `pkg-config --libs gtk+-2.0` `pkg-config --libs fftw3` -L/usr/lib -larpack  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/features

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/features: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -L/usr/lib -larpack -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/features -s ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/sift.o: sift.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/sift.o sift.c

${OBJECTDIR}/image.o: image.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/image.o image.c

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/matrixUtils.o: matrixUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/matrixUtils.o matrixUtils.cpp

${OBJECTDIR}/discretisation.o: discretisation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/discretisation.o discretisation.cpp

${OBJECTDIR}/computeEdges.o: computeEdges.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/computeEdges.o computeEdges.cpp

${OBJECTDIR}/spmtimesd.o: spmtimesd.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/spmtimesd.o spmtimesd.cpp

${OBJECTDIR}/sparsifyc.o: sparsifyc.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/sparsifyc.o sparsifyc.cpp

${OBJECTDIR}/feature.o: feature.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/feature.o feature.c

${OBJECTDIR}/ncutW.o: ncutW.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/ncutW.o ncutW.cpp

${OBJECTDIR}/utils.o: utils.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/utils.o utils.c

${OBJECTDIR}/tools.o: tools.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/tools.o tools.c

${OBJECTDIR}/computeW.o: computeW.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/computeW.o computeW.cpp

${OBJECTDIR}/imgfeatures.o: imgfeatures.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/imgfeatures.o imgfeatures.c

${OBJECTDIR}/ICgraph.o: ICgraph.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Iarpack++/include -IEigen `pkg-config --cflags opencv` `pkg-config --cflags gtk+-2.0` `pkg-config --cflags fftw3`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/ICgraph.o ICgraph.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/features

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
