/*---------------------------------------------------------------------------*\

  FILE........: gp_interleaver.c
  AUTHOR......: David Rowe
  DATE CREATED: April 2018

  Golden Prime Interleaver. My interpretation of "On the Analysis and
  Design of Good Algebraic Interleavers", Xie et al,eq (5).

  See also octave/gp_interleaver.m

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2018 David Rowe

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

#include "gp_interleaver.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

/*
  Choose b for Golden Prime Interleaver.  b is chosen to be the
  closest integer, which is relatively prime to N, to the Golden
  section of N.
*/

int is_prime(int x) {
  for (int i = 2; i < x; i++) {
    if ((x % i) == 0) return 0;
  }
  return 1;
}

int next_prime(int x) {
  x++;
  while (is_prime(x) == 0) x++;
  return x;
}

int choose_interleaver_b(int Nbits) {
  int b = floor(Nbits / 1.62);
  b = next_prime(b);
  return b;
}

void gp_interleave_comp(COMP interleaved_frame[], COMP frame[], int Nbits) {
  int b = choose_interleaver_b(Nbits);
  int i, j;
  for (i = 0; i < Nbits; i++) {
    j = (b * i) % Nbits;
    interleaved_frame[j] = frame[i];
  }
}

void gp_deinterleave_comp(COMP frame[], COMP interleaved_frame[], int Nbits) {
  int b = choose_interleaver_b(Nbits);
  int i, j;
  for (i = 0; i < Nbits; i++) {
    j = (b * i) % Nbits;
    frame[i] = interleaved_frame[j];
  }
}

void gp_interleave_float(float interleaved_frame[], float frame[], int Nbits) {
  int b = choose_interleaver_b(Nbits);
  int i, j;

  for (i = 0; i < Nbits; i++) {
    j = (b * i) % Nbits;
    interleaved_frame[j] = frame[i];
  }
}

void gp_deinterleave_float(float frame[], float interleaved_frame[],
                           int Nbits) {
  int b = choose_interleaver_b(Nbits);
  int i, j;

  for (i = 0; i < Nbits; i++) {
    j = (b * i) % Nbits;
    frame[i] = interleaved_frame[j];
  }
}

// The above work on complex numbers (e.g. OFDM symbols), so the below work on
// groups of two bits at a time to remain compatible with the above.
void gp_interleave_bits(char interleaved_frame[], char frame[], int Nbits) {
  char temp[Nbits];
  int b = choose_interleaver_b(Nbits);
  int i, j;

  for (i = 0; i < Nbits; i++) {
    j = (b * i) % Nbits;
    temp[j] = ((frame[i * 2] & 1) << 1) | (frame[i * 2 + 1] & 1);
  }

  for (i = 0; i < Nbits; i++) {
    interleaved_frame[i * 2] = temp[i] >> 1;
    interleaved_frame[i * 2 + 1] = temp[i] & 1;
  }
}

void gp_deinterleave_bits(char frame[], char interleaved_frame[], int Nbits) {
  char temp[Nbits];
  int b = choose_interleaver_b(Nbits);
  int i, j;

  for (i = 0; i < Nbits; i++) {
    j = (b * i) % Nbits;
    temp[i] = ((interleaved_frame[j * 2] & 1) << 1) |
              (interleaved_frame[j * 2 + 1] & 1);
  }

  for (i = 0; i < Nbits; i++) {
    frame[i * 2] = temp[i] >> 1;
    frame[i * 2 + 1] = temp[i] & 1;
  }
}
