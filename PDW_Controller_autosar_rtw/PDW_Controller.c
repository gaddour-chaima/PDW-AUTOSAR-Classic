/*
 * File: PDW_Controller.c
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

#include "PDW_Controller.h"
#include "rtwtypes.h"

/* PublicStructure Variables for Internal Data */
ARID_DEF_PDW_Controller_T PDW_Controller_ARID_DEF;/* '<S1>/Tick_Delay' */

/* Model step function */
void PDW_Main_20ms(void)
{
  uint8 rtb_Switch_Reverse;

  /* Switch: '<S3>/Switch_Reverse' incorporates:
   *  Inport: '<Root>/ReverseGearInput'
   */
  if (Rte_IRead_PDW_Main_20ms_ReverseGearInput_ReverseGearInput()) {
    float32 tmp;

    /* RelationalOperator: '<S9>/Compare' incorporates:
     *  Inport: '<Root>/DistanceInput'
     *  RelationalOperator: '<S12>/Compare'
     *  Switch: '<S3>/Switch_150'
     *  Switch: '<S3>/Switch_80'
     *  Switch: '<S3>/Switch_Valid'
     */
    tmp = Rte_IRead_PDW_Main_20ms_DistanceInput_DistanceInput();

    /* Switch: '<S3>/Switch_Valid' incorporates:
     *  Constant: '<S10>/Constant'
     *  Constant: '<S9>/Constant'
     *  Inport: '<Root>/DistanceInput'
     *  Logic: '<S2>/Logical Operator'
     *  RelationalOperator: '<S10>/Compare'
     *  RelationalOperator: '<S9>/Compare'
     */
    if ((tmp >= 0.0F) && (tmp <= 4.0F)) {
      /* Switch: '<S3>/Switch_150' incorporates:
       *  Constant: '<S11>/Constant'
       *  Constant: '<S12>/Constant'
       *  Constant: '<S13>/Constant'
       *  RelationalOperator: '<S11>/Compare'
       *  RelationalOperator: '<S12>/Compare'
       *  RelationalOperator: '<S13>/Compare'
       *  Switch: '<S3>/Switch_30'
       *  Switch: '<S3>/Switch_80'
       */
      if (tmp > 1.5F) {
        /* Switch: '<S3>/Switch_Reverse' incorporates:
         *  Constant: '<S3>/level_0'
         *  Switch: '<S3>/Switch_150'
         */
        rtb_Switch_Reverse = 0U;
      } else if (tmp >= 0.8F) {
        /* Switch: '<S3>/Switch_Reverse' incorporates:
         *  Constant: '<S3>/level_1'
         *  Switch: '<S3>/Switch_150'
         *  Switch: '<S3>/Switch_80'
         */
        rtb_Switch_Reverse = 1U;
      } else if (tmp >= 0.3F) {
        /* Switch: '<S3>/Switch_30' incorporates:
         *  Constant: '<S3>/level_2'
         *  Switch: '<S3>/Switch_150'
         *  Switch: '<S3>/Switch_80'
         *  Switch: '<S3>/Switch_Reverse'
         */
        rtb_Switch_Reverse = 2U;
      } else {
        /* Switch: '<S3>/Switch_Reverse' incorporates:
         *  Constant: '<S3>/level_3'
         *  Switch: '<S3>/Switch_150'
         *  Switch: '<S3>/Switch_30'
         *  Switch: '<S3>/Switch_80'
         */
        rtb_Switch_Reverse = 3U;
      }
    } else {
      /* Switch: '<S3>/Switch_Reverse' incorporates:
       *  Constant: '<S3>/level_4_Fault'
       *  Switch: '<S3>/Switch_Valid'
       */
      rtb_Switch_Reverse = 4U;
    }
  } else {
    /* Switch: '<S3>/Switch_Reverse' incorporates:
     *  Constant: '<S3>/level_0'
     */
    rtb_Switch_Reverse = 0U;
  }

  /* End of Switch: '<S3>/Switch_Reverse' */

  /* Switch: '<S1>/Switch_Reset' incorporates:
   *  RelationalOperator: '<S1>/Level_Changed'
   *  UnitDelay: '<S1>/Previous_Level'
   */
  if (rtb_Switch_Reverse != PDW_Controller_ARID_DEF.Previous_Level_DSTATE) {
    /* Sum: '<S1>/Add_One' incorporates:
     *  Constant: '<S1>/Constant'
     */
    PDW_Controller_ARID_DEF.Tick_Delay_DSTATE = 0U;
  }

  /* End of Switch: '<S1>/Switch_Reset' */

  /* Outport: '<Root>/BuzzerOutput' incorporates:
   *  Constant: '<S4>/Constant'
   *  Constant: '<S5>/Constant'
   *  Constant: '<S6>/Constant'
   *  Constant: '<S7>/Constant'
   *  Constant: '<S8>/Constant'
   *  Logic: '<S1>/AND_Fast'
   *  Logic: '<S1>/AND_Slow'
   *  Logic: '<S1>/OR_Buzzer'
   *  Math: '<S1>/Mod_10'
   *  RelationalOperator: '<S4>/Compare'
   *  RelationalOperator: '<S5>/Compare'
   *  RelationalOperator: '<S6>/Compare'
   *  RelationalOperator: '<S7>/Compare'
   *  RelationalOperator: '<S8>/Compare'
   */
  Rte_IWrite_PDW_Main_20ms_BuzzerOutput_BuzzerOutput(((rtb_Switch_Reverse == 1) &&
    (PDW_Controller_ARID_DEF.Tick_Delay_DSTATE < 25)) || ((rtb_Switch_Reverse ==
    2) && ((uint16)(PDW_Controller_ARID_DEF.Tick_Delay_DSTATE % 10) < 5)) ||
    (rtb_Switch_Reverse == 3));

  /* Math: '<S1>/Mod_50' incorporates:
   *  Constant: '<S1>/Constant1'
   *  Sum: '<S1>/Add_One'
   */
  PDW_Controller_ARID_DEF.Tick_Delay_DSTATE = (uint16)((uint16)
    (PDW_Controller_ARID_DEF.Tick_Delay_DSTATE + 1) % 50);

  /* Outport: '<Root>/WarningLevelOutput' */
  Rte_IWrite_PDW_Main_20ms_WarningLevelOutput_WarningLevelOutput
    (rtb_Switch_Reverse);

  /* Update for UnitDelay: '<S1>/Previous_Level' */
  PDW_Controller_ARID_DEF.Previous_Level_DSTATE = rtb_Switch_Reverse;
}

/* Model initialize function */
void PDW_Controller_Init(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
