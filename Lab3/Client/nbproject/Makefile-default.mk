#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../Lab3/aux/Client/crc16.c ../Lab3/aux/Client/main.c ../Lab3/aux/Client/timer.c ../Lab3/aux/Client/uart.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/593570672/crc16.o ${OBJECTDIR}/_ext/593570672/main.o ${OBJECTDIR}/_ext/593570672/timer.o ${OBJECTDIR}/_ext/593570672/uart.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/593570672/crc16.o.d ${OBJECTDIR}/_ext/593570672/main.o.d ${OBJECTDIR}/_ext/593570672/timer.o.d ${OBJECTDIR}/_ext/593570672/uart.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/593570672/crc16.o ${OBJECTDIR}/_ext/593570672/main.o ${OBJECTDIR}/_ext/593570672/timer.o ${OBJECTDIR}/_ext/593570672/uart.o

# Source Files
SOURCEFILES=../Lab3/aux/Client/crc16.c ../Lab3/aux/Client/main.c ../Lab3/aux/Client/timer.c ../Lab3/aux/Client/uart.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33FJ256MC710
MP_LINKER_FILE_OPTION=,--script=p33FJ256MC710.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/593570672/crc16.o: ../Lab3/aux/Client/crc16.c  .generated_files/flags/default/ca16498534d3a1040ef7a080ef443f3b55c8db87 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/593570672" 
	@${RM} ${OBJECTDIR}/_ext/593570672/crc16.o.d 
	@${RM} ${OBJECTDIR}/_ext/593570672/crc16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Lab3/aux/Client/crc16.c  -o ${OBJECTDIR}/_ext/593570672/crc16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/593570672/crc16.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/593570672/main.o: ../Lab3/aux/Client/main.c  .generated_files/flags/default/6da72221b284adeee0db0d302c28f78199f05fe7 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/593570672" 
	@${RM} ${OBJECTDIR}/_ext/593570672/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/593570672/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Lab3/aux/Client/main.c  -o ${OBJECTDIR}/_ext/593570672/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/593570672/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/593570672/timer.o: ../Lab3/aux/Client/timer.c  .generated_files/flags/default/8d1eb25c9fedef355882f27c860d903e3a7c8904 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/593570672" 
	@${RM} ${OBJECTDIR}/_ext/593570672/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/593570672/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Lab3/aux/Client/timer.c  -o ${OBJECTDIR}/_ext/593570672/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/593570672/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/593570672/uart.o: ../Lab3/aux/Client/uart.c  .generated_files/flags/default/5e7b338eb907e709fefc201d515ef7e0c73523d5 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/593570672" 
	@${RM} ${OBJECTDIR}/_ext/593570672/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/593570672/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Lab3/aux/Client/uart.c  -o ${OBJECTDIR}/_ext/593570672/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/593570672/uart.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/593570672/crc16.o: ../Lab3/aux/Client/crc16.c  .generated_files/flags/default/5afeeec94934234716a9f32817cd6ca75de69d12 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/593570672" 
	@${RM} ${OBJECTDIR}/_ext/593570672/crc16.o.d 
	@${RM} ${OBJECTDIR}/_ext/593570672/crc16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Lab3/aux/Client/crc16.c  -o ${OBJECTDIR}/_ext/593570672/crc16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/593570672/crc16.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/593570672/main.o: ../Lab3/aux/Client/main.c  .generated_files/flags/default/436180ccffe561064d16c66714da7e5ce450623e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/593570672" 
	@${RM} ${OBJECTDIR}/_ext/593570672/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/593570672/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Lab3/aux/Client/main.c  -o ${OBJECTDIR}/_ext/593570672/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/593570672/main.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/593570672/timer.o: ../Lab3/aux/Client/timer.c  .generated_files/flags/default/b8309cdc2742c0b3d98b2297ba7b71bd3fabc0d9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/593570672" 
	@${RM} ${OBJECTDIR}/_ext/593570672/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/593570672/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Lab3/aux/Client/timer.c  -o ${OBJECTDIR}/_ext/593570672/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/593570672/timer.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/593570672/uart.o: ../Lab3/aux/Client/uart.c  .generated_files/flags/default/dceb3f02c6bc3bd5ed99c1927ec9170a54307c2 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/593570672" 
	@${RM} ${OBJECTDIR}/_ext/593570672/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/593570672/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../Lab3/aux/Client/uart.c  -o ${OBJECTDIR}/_ext/593570672/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/593570672/uart.o.d"        -g -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  lcd.a  
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    lcd.a  -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  lcd.a 
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}    lcd.a  -mcpu=$(MP_PROCESSOR_OPTION)        -omf=coff -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}/xc16-bin2hex ${DISTDIR}/Lab3.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=coff   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
