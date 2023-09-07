/*---------------------------------------------------------------------------*\

  FILE........: defines.h
  AUTHOR......: David Rowe
  DATE CREATED: 23/4/93

  Defines and structures used throughout the codec.

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2009 David Rowe

  All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License version 2.1, as
  published by the Free Software Foundation.  This program is
  distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __DEFINES__
#define __DEFINES__

/*---------------------------------------------------------------------------*\

                                DEFINES

\*---------------------------------------------------------------------------*/

/* VLA workaround */

#if defined(__STDC_NO_VLA__) || defined(_MSC_VER)
#include "stdlib.h"
#include "string.h"

#define VLA_CALLOC(type, name, len) \
  type* name = (type*)calloc(len, sizeof(type))

#define VLA_CALLOC2(type, name1, name2, len) \
  VLA_CALLOC(type, name1, len);              \
  VLA_CALLOC(type, name2, len)

#define VLA_CALLOC3(type, name1, name2, name3, len) \
  VLA_CALLOC2(type, name1, name2, len);             \
  VLA_CALLOC(type, name3, len)

#define VLA_CALLOC4(type, name1, name2, name3, name4, len) \
  VLA_CALLOC3(type, name1, name2, name3, len);             \
  VLA_CALLOC(type, name4, len)

// Helper macro to free a single pointer
#define VLA_FREE_SINGLE_PTR(ptr) \
  do {                           \
    if (ptr != NULL) {           \
      free(ptr);                 \
      ptr = NULL;                \
    }                            \
  } while (0)

// Variadic macro to free multiple pointers
#define VLA_FREE(...)                                       \
  do {                                                      \
    void* ptrs__[] = {__VA_ARGS__};                         \
    size_t num_ptrs__ = sizeof(ptrs__) / sizeof(ptrs__[0]); \
    for (size_t i__ = 0; i__ < num_ptrs__; i__++) {         \
      VLA_FREE_SINGLE_PTR(ptrs__[i__]);                     \
    }                                                       \
  } while (0)

#define VLA_LEN_DIM1_NAME(name) name##_VLA_DIM1_LENGTH_XY__

#define VLA_CALLOC_DIM2(type, name, len1, len2)      \
  type** name = (type**)calloc(len1, sizeof(type*)); \
  const int VLA_LEN_DIM1_NAME(name) = len1;          \
  do {                                               \
    for (int i__ = 0; i__ < len1; i__++) {           \
      name[i__] = (type*)calloc(len2, sizeof(type)); \
    }                                                \
                                                     \
  } while (0)

#define VLA_FREE_DIM2(name)                                   \
  do {                                                        \
    for (int i__ = 0; i__ < VLA_LEN_DIM1_NAME(name); i__++) { \
      VLA_FREE_SINGLE_PTR(name[i__]);                         \
    }                                                         \
    VLA_FREE_SINGLE_PTR(name);                                \
  } while (0)

#else

#define VLA_CALLOC(type, name, len) type name[len]

#define VLA_CALLOC2(type, name1, name2, len) \
  VLA_CALLOC(type, name1, len);              \
  VLA_CALLOC(type, name2, len)

#define VLA_CALLOC3(type, name1, name2, name3, len) \
  VLA_CALLOC2(type, name1, name2, len);             \
  VLA_CALLOC(type, name3, len)

#define VLA_CALLOC4(type, name1, name2, name3, name4, len) \
  VLA_CALLOC3(type, name1, name2, name3, len);             \
  VLA_CALLOC(type, name4, len)

#define VLA_FREE(...)

#define VLA_CALLOC_DIM2(type, name, len1, len2) type name[len1][len2]
#define VLA_FREE_DIM2(name)

#endif

/* General defines */

#define N_S 0.01    /* internal proc frame length in secs   */
#define TW_S 0.005  /* trapezoidal synth window overlap     */
#define MAX_AMP 160 /* maximum number of harmonics          */
#ifndef PI
#define PI 3.141592654 /* mathematical constant                */
#endif
#define TWO_PI 6.283185307 /* mathematical constant                */
#define MAX_STR 2048       /* maximum string size                  */

#define FFT_ENC 512   /* size of FFT used for encoder         */
#define FFT_DEC 512   /* size of FFT used in decoder          */
#define V_THRESH 6.0  /* voicing threshold in dB              */
#define LPC_ORD 10    /* LPC order                            */
#define LPC_ORD_LOW 6 /* LPC order for lower rates            */

/* Pitch estimation defines */

#define M_PITCH_S 0.0400 /* pitch analysis window in s           */
#define P_MIN_S 0.0025   /* minimum pitch period in s            */
#define P_MAX_S 0.0200   /* maximum pitch period in s            */

/*---------------------------------------------------------------------------*\

                                TYPEDEFS

\*---------------------------------------------------------------------------*/

/* Structure to hold constants calculated at run time based on sample rate
 */

typedef struct {
  int Fs;      /* sample rate of this instance             */
  int n_samp;  /* number of samples per 10ms frame at Fs   */
  int max_amp; /* maximum number of harmonics              */
  int m_pitch; /* pitch estimation window size in samples  */
  int p_min;   /* minimum pitch period in samples          */
  int p_max;   /* maximum pitch period in samples          */
  float Wo_min;
  float Wo_max;
  int nw; /* analysis window size in samples          */
  int tw; /* trapezoidal synthesis window overlap     */
} C2CONST;

/* Structure to hold model parameters for one frame */

typedef struct {
  float Wo;               /* fundamental frequency estimate in radians  */
  int L;                  /* number of harmonics                        */
  float A[MAX_AMP + 1];   /* amplitiude of each harmonic                */
  float phi[MAX_AMP + 1]; /* phase of each harmonic                     */
  int voiced;             /* non-zero if this frame is voiced           */
} MODEL;

/* describes each codebook  */

struct lsp_codebook {
  int k;            /* dimension of vector	*/
  int log2m;        /* number of bits in m	*/
  int m;            /* elements in codebook	*/
#ifdef __EMBEDDED__ /* make sure stored in flash  */
  const float* cb;  /* The elements		*/
#else
  float* cb; /* The elements		*/
#endif
};

extern const struct lsp_codebook lsp_cb[];
extern const struct lsp_codebook lsp_cbd[];
extern const struct lsp_codebook lsp_cbjmv[];
extern const struct lsp_codebook ge_cb[];
extern const struct lsp_codebook newamp1vq_cb[];
extern const struct lsp_codebook newamp1_energy_cb[];
extern const struct lsp_codebook newamp2vq_cb[];
extern const struct lsp_codebook newamp2_energy_cb[];

#ifdef _GNU_SOURCE
#define POW10F(x) exp10f((x))
#else
#define POW10F(x) expf(2.302585092994046f * (x))
#endif

#endif