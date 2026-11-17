/*
 * File: PDW_Controller.h
 *
 * Code generated for Simulink model 'PDW_Controller'.
 *
 * Model version                  : 1.30
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Oct 28 22:56:37 2026
 *
 * Target selection: autosar.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_PDW_Controller_h_
#define RTW_HEADER_PDW_Controller_h_
#ifndef PDW_Controller_COMMON_INCLUDES_
#define PDW_Controller_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "Rte_ParkingDistanceWarning.h"
#endif                                 /* PDW_Controller_COMMON_INCLUDES_ */

#include "PDW_Controller_types.h"

/* PublicStructure Variables for Internal Data, for system '<Root>' */
typedef struct {
  uint16 Tick_Delay_DSTATE;            /* '<S1>/Tick_Delay' */
  uint8 Previous_Level_DSTATE;         /* '<S1>/Previous_Level' */
} ARID_DEF_PDW_Controller_T;

/* PublicStructure Variables for Internal Data */
extern ARID_DEF_PDW_Controller_T PDW_Controller_ARID_DEF;/* '<S1>/Tick_Delay' */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'PDW_Controller'
 * '<S1>'   : 'PDW_Controller/Buzzer_Generator'
 * '<S2>'   : 'PDW_Controller/Sensor_Validation'
 * '<S3>'   : 'PDW_Controller/Warning_Decision'
 * '<S4>'   : 'PDW_Controller/Buzzer_Generator/Fast_Phase'
 * '<S5>'   : 'PDW_Controller/Buzzer_Generator/Is_Continuous'
 * '<S6>'   : 'PDW_Controller/Buzzer_Generator/Is_Fast'
 * '<S7>'   : 'PDW_Controller/Buzzer_Generator/Is_Slow'
 * '<S8>'   : 'PDW_Controller/Buzzer_Generator/Slow_Phase'
 * '<S9>'   : 'PDW_Controller/Sensor_Validation/Compare To Constant'
 * '<S10>'  : 'PDW_Controller/Sensor_Validation/Compare To Constant1'
 * '<S11>'  : 'PDW_Controller/Warning_Decision/Cmp_150'
 * '<S12>'  : 'PDW_Controller/Warning_Decision/Cmp_30'
 * '<S13>'  : 'PDW_Controller/Warning_Decision/Cmp_80'
 */
#endif                                 /* RTW_HEADER_PDW_Controller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
