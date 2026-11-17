/* This file contains stub implementations of the AUTOSAR RTE functions.
   The stub implementations can be used for testing the generated code in
   Simulink, for example, in SIL/PIL simulations of the component under
   test. Note that this file should be replaced with an appropriate RTE
   file when deploying the generated code outside of Simulink.

   This file is generated for:
   Atomic software component:  "ParkingDistanceWarning"
   ARXML schema: "R20-11"
   File generated on: "28-Oct-2026 22:56:59"  */

#ifndef Rte_ParkingDistanceWarning_h
#define Rte_ParkingDistanceWarning_h
#include "Rte_Type.h"
#include "Compiler.h"

/* Data access functions */
#define Rte_IRead_PDW_Main_20ms_DistanceInput_DistanceInput Rte_IRead_ParkingDistanceWarning_PDW_Main_20ms_DistanceInput_DistanceInput

Float Rte_IRead_PDW_Main_20ms_DistanceInput_DistanceInput(void);

#define Rte_IRead_PDW_Main_20ms_ReverseGearInput_ReverseGearInput Rte_IRead_ParkingDistanceWarning_PDW_Main_20ms_ReverseGearInput_ReverseGearInput

Boolean Rte_IRead_PDW_Main_20ms_ReverseGearInput_ReverseGearInput(void);

#define Rte_IWrite_PDW_Main_20ms_WarningLevelOutput_WarningLevelOutput Rte_IWrite_ParkingDistanceWarning_PDW_Main_20ms_WarningLevelOutput_WarningLevelOutput

void Rte_IWrite_PDW_Main_20ms_WarningLevelOutput_WarningLevelOutput(UInt8 u);

#define Rte_IWriteRef_PDW_Main_20ms_WarningLevelOutput_WarningLevelOutput Rte_IWriteRef_ParkingDistanceWarning_PDW_Main_20ms_WarningLevelOutput_WarningLevelOutput

UInt8* Rte_IWriteRef_PDW_Main_20ms_WarningLevelOutput_WarningLevelOutput(void);

#define Rte_IWrite_PDW_Main_20ms_BuzzerOutput_BuzzerOutput Rte_IWrite_ParkingDistanceWarning_PDW_Main_20ms_BuzzerOutput_BuzzerOutput

void Rte_IWrite_PDW_Main_20ms_BuzzerOutput_BuzzerOutput(Boolean u);

#define Rte_IWriteRef_PDW_Main_20ms_BuzzerOutput_BuzzerOutput Rte_IWriteRef_ParkingDistanceWarning_PDW_Main_20ms_BuzzerOutput_BuzzerOutput

Boolean* Rte_IWriteRef_PDW_Main_20ms_BuzzerOutput_BuzzerOutput(void);

/* Entry point functions */
extern FUNC(void, ParkingDistanceWarning_CODE) PDW_Controller_Init(void);
extern FUNC(void, ParkingDistanceWarning_CODE) PDW_Main_20ms(void);

#endif
