/*
 * Copyright(C) 2007, 2013 Renesas Electronics Corporation
 * RENESAS ELECTRONICS CONFIDENTIAL AND PROPRIETARY.
 * This program must be used solely for the purpose for which
 * it was furnished by Renesas Electronics Corporation. No part of this
 * program may be reproduced or disclosed to others, in any
 * form, without the prior written permission of Renesas Electronics
 * Corporation.
 */

/********************************************************************************/
/* RX Family C/C++ Compiler                                                     */
/********************************************************************************/

#ifndef _MACRO
#define _MACRO

#ifdef __RX

#ifndef _NO_MT
#define _NO_MT
#endif /* !_NO_MT */

#ifdef __STDC_VERSION__
#if 199901L <= __STDC_VERSION__
#define _C99
#endif /* 199901L <= __STDC_VERSION__ */
#endif /* __STDC_VERSION__ */

#ifndef _HAS_TR1
#define _HAS_TR1 0
#endif /* !_HAS_TR1 */

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 0
#endif /* !__STDC_WANT_LIB_EXT1 */

#ifndef _ADDED_C_LIB
#define _ADDED_C_LIB 0
#endif /* !_ADDED_C_LIB */

#ifndef __STDC_WANT_SAFER_LIB__
#define __STDC_WANT_SAFER_LIB__ 0
#endif /* !__STDC_WANT_SAFER_LIB__ */

#ifndef _ALT_NS
#define _ALT_NS 0
#endif /* !_ALT_NS */

#ifdef __cplusplus

#ifndef _C99
#define _C99
#endif /* !_C99*/ 

#endif /* __cplusplus */

#ifdef __embedded_cplusplus

#ifndef _NO_NS
#define _NO_NS
#endif /* !_NO_NS */

#ifndef _ECPP
#define _ECPP
#endif /* !_ECPP */

#endif /* __embedded_cplusplus */

#ifdef _REENTRANT

#ifndef _R_REENT
#define _R_REENT
#endif /* !_R_REENT */

#endif /* _REENTRANT */

#ifndef __ISA_HAS_FSQRT_INST
#if !defined(__RX600) && !defined(__RX200) && !defined(__RXV1)
#define __ISA_HAS_FSQRT_INST
#endif /* !defined(__RX600) && !defined(__RX200) && !defined(__RXV1) */
#endif /* !__ISA_HAS_FSQRT_INST */

#endif /* __RX */

#endif /* _MACRO */
