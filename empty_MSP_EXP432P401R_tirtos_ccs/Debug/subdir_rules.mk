################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/rensv/workspace_v12/Embedded_systems/empty_MSP_EXP432P401R_tirtos_ccs" --include_path="C:/Users/rensv/workspace_v12/Embedded_systems/empty_MSP_EXP432P401R_tirtos_ccs/Debug" --include_path="/source" --include_path="/source/third_party/CMSIS/Include" --include_path="/source/ti/posix/ccs" --include_path="C:/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=none -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/rensv/workspace_v12/Embedded_systems/empty_MSP_EXP432P401R_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-97563279: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1220/ccs/utils/sysconfig_1.15.0/sysconfig_cli.bat" --script "C:/Users/rensv/workspace_v12/Embedded_systems/empty_MSP_EXP432P401R_tirtos_ccs/empty.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/error.h: build-97563279 ../empty.syscfg
syscfg/: build-97563279


