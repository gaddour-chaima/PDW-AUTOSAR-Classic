/*
 * File: coder_assumptions.h
 *
 * Abstract: Coder assumptions header file
 */

#ifndef CODER_ASSUMPTIONS_H
#define CODER_ASSUMPTIONS_H

/* include model specific checks */
#include "PDW_Controller_ca.h"

/* global results variable mapping for static code */
#define CA_Expected_HWImpl             CA_PDW_Controller_ExpHW
#define CA_Actual_HWImpl               CA_PDW_Controller_ActHW
#define CA_HWImpl_Results              CA_PDW_Controller_HWRes
#define CA_PortableWordSizes_Results   CA_PDW_Controller_PWSRes

/* entry point function mapping for static code */
#define CA_Run_Tests                   PDW_Controller_caRunTests
#endif                                 /* CODER_ASSUMPTIONS_H */
