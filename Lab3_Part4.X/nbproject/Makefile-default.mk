#
# Generated Makefile - do not edit!
# Rebuilt for the Lab 3 per-part projects.
#

ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif

MKDIR=mkdir

CND_CONF=default
PROJECTNAME=Lab3_Part4.X
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

SOURCEFILES_QUOTED_IF_SPACED=Part4_L298_DC.c ../src/BOARD.c ../src/AD.c ../src/IO_Ports.c ../src/LED.c ../src/pwm.c ../src/serial.c ../src/timers.c
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Part4_L298_DC.o ${OBJECTDIR}/_ext/1360937237/BOARD.o ${OBJECTDIR}/_ext/1360937237/AD.o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ${OBJECTDIR}/_ext/1360937237/LED.o ${OBJECTDIR}/_ext/1360937237/pwm.o ${OBJECTDIR}/_ext/1360937237/serial.o ${OBJECTDIR}/_ext/1360937237/timers.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Part4_L298_DC.o.d ${OBJECTDIR}/_ext/1360937237/BOARD.o.d ${OBJECTDIR}/_ext/1360937237/AD.o.d ${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d ${OBJECTDIR}/_ext/1360937237/LED.o.d ${OBJECTDIR}/_ext/1360937237/pwm.o.d ${OBJECTDIR}/_ext/1360937237/serial.o.d ${OBJECTDIR}/_ext/1360937237/timers.o.d
OBJECTFILES=${OBJECTDIR}/Part4_L298_DC.o ${OBJECTDIR}/_ext/1360937237/BOARD.o ${OBJECTDIR}/_ext/1360937237/AD.o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ${OBJECTDIR}/_ext/1360937237/LED.o ${OBJECTDIR}/_ext/1360937237/pwm.o ${OBJECTDIR}/_ext/1360937237/serial.o ${OBJECTDIR}/_ext/1360937237/timers.o
SOURCEFILES=Part4_L298_DC.c ../src/BOARD.c ../src/AD.c ../src/IO_Ports.c ../src/LED.c ../src/pwm.c ../src/serial.c ../src/timers.c

CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=
FIXDEPS=fixDeps

.build-conf: ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE} -f nbproject/Makefile-default.mk ${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=,--script="..\bootloader320.ld"

${OBJECTDIR}/Part4_L298_DC.o: Part4_L298_DC.c
	@if not exist "${OBJECTDIR}" mkdir "${OBJECTDIR}"
	${MP_CC} $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../include" -O1 -fno-common -MP -MMD -MF "${OBJECTDIR}/Part4_L298_DC.o.d" -o ${OBJECTDIR}/Part4_L298_DC.o Part4_L298_DC.c -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -mdfp="${DFP_DIR}"

${OBJECTDIR}/_ext/1360937237/BOARD.o: ../src/BOARD.c
	@if not exist "${OBJECTDIR}/_ext/1360937237" mkdir "${OBJECTDIR}/_ext/1360937237"
	${MP_CC} $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../include" -O1 -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/BOARD.o.d" -o ${OBJECTDIR}/_ext/1360937237/BOARD.o ../src/BOARD.c -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -mdfp="${DFP_DIR}"

${OBJECTDIR}/_ext/1360937237/AD.o: ../src/AD.c
	@if not exist "${OBJECTDIR}/_ext/1360937237" mkdir "${OBJECTDIR}/_ext/1360937237"
	${MP_CC} $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../include" -O1 -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/AD.o.d" -o ${OBJECTDIR}/_ext/1360937237/AD.o ../src/AD.c -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -mdfp="${DFP_DIR}"

${OBJECTDIR}/_ext/1360937237/IO_Ports.o: ../src/IO_Ports.c
	@if not exist "${OBJECTDIR}/_ext/1360937237" mkdir "${OBJECTDIR}/_ext/1360937237"
	${MP_CC} $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../include" -O1 -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/IO_Ports.o.d" -o ${OBJECTDIR}/_ext/1360937237/IO_Ports.o ../src/IO_Ports.c -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -mdfp="${DFP_DIR}"

${OBJECTDIR}/_ext/1360937237/LED.o: ../src/LED.c
	@if not exist "${OBJECTDIR}/_ext/1360937237" mkdir "${OBJECTDIR}/_ext/1360937237"
	${MP_CC} $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../include" -O1 -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/LED.o.d" -o ${OBJECTDIR}/_ext/1360937237/LED.o ../src/LED.c -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -mdfp="${DFP_DIR}"

${OBJECTDIR}/_ext/1360937237/pwm.o: ../src/pwm.c
	@if not exist "${OBJECTDIR}/_ext/1360937237" mkdir "${OBJECTDIR}/_ext/1360937237"
	${MP_CC} $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../include" -O1 -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/pwm.o.d" -o ${OBJECTDIR}/_ext/1360937237/pwm.o ../src/pwm.c -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -mdfp="${DFP_DIR}"

${OBJECTDIR}/_ext/1360937237/serial.o: ../src/serial.c
	@if not exist "${OBJECTDIR}/_ext/1360937237" mkdir "${OBJECTDIR}/_ext/1360937237"
	${MP_CC} $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../include" -O1 -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/serial.o.d" -o ${OBJECTDIR}/_ext/1360937237/serial.o ../src/serial.c -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -mdfp="${DFP_DIR}"

${OBJECTDIR}/_ext/1360937237/timers.o: ../src/timers.c
	@if not exist "${OBJECTDIR}/_ext/1360937237" mkdir "${OBJECTDIR}/_ext/1360937237"
	${MP_CC} $(MP_EXTRA_CC_PRE) -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../include" -O1 -fno-common -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/timers.o.d" -o ${OBJECTDIR}/_ext/1360937237/timers.o ../src/timers.c -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -mdfp="${DFP_DIR}"

${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES} nbproject/Makefile-${CND_CONF}.mk ../bootloader320.ld
	@if not exist "${DISTDIR}" mkdir "${DISTDIR}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mprocessor=$(MP_PROCESSOR_OPTION) -o ${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED} -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
else
	${MP_CC} $(MP_EXTRA_LD_PRE) -mprocessor=$(MP_PROCESSOR_OPTION) -o ${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED} -DXPRJ_default=$(CND_CONF) $(COMPARISON_BUILD) -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}
endif

.build-subprojects:
.clean-subprojects:

.clean-conf: ${CLEAN_SUBPROJECTS}
	cmd /c if exist build rmdir /s /q build
	cmd /c if exist dist rmdir /s /q dist





