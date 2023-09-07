/*---------------------------------------------------------------------------*\

  FILE........: codec2_ofdm.h
  AUTHORS.....: David Rowe & Steve Sampson
  DATE CREATED: June 2017

  External user references to the modem library.

\*---------------------------------------------------------------------------*/

/*
  Copyright (C) 2017 David Rowe

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

#ifndef CODEC2_OFDM_H
#define CODEC2_OFDM_H

/* Includes */

#include <stdbool.h>
#include <stdint.h>

#include "comp.h"
#include "export.h"
#include "modem_stats.h"

/* Defines */

#ifdef __cplusplus
extern "C" {
#endif

#define OFDM_PEAK 16384 /* peak level of OFDM TX signal */
#define OFDM_CLIP \
  (32767 *        \
   0.35) /* experimentally derived constant to reduce PAPR to about 8dB */

#define UN_SYNC 0 /* Used with the ofdm_set_sync() */
#define AUTO_SYNC 1
#define MANUAL_SYNC 2

#define AUTO_PHASE_EST 0
#define LOCKED_PHASE_EST 1

#define LOW_BW 0
#define HIGH_BW 1

struct OFDM_CONFIG;
struct OFDM;

/* create and destroy modem states */

CODEC2_API struct OFDM *ofdm_create(const struct OFDM_CONFIG *config);
CODEC2_API void ofdm_destroy(struct OFDM *);
CODEC2_API void ofdm_init_mode(char mode[], struct OFDM_CONFIG *config);

/* signal processing */

CODEC2_API void ofdm_mod(struct OFDM *, COMP *, const int *);
CODEC2_API void ofdm_demod(struct OFDM *, int *, COMP *);
CODEC2_API void ofdm_demod_shorts(struct OFDM *, int *, short *, float);
CODEC2_API int ofdm_sync_search(struct OFDM *, COMP *);
CODEC2_API int ofdm_sync_search_shorts(struct OFDM *, short *, float);
CODEC2_API void ofdm_sync_state_machine(struct OFDM *, uint8_t *);
CODEC2_API void ofdm_sync_state_machine2(struct OFDM *, uint8_t *);

/* getters */

CODEC2_API struct OFDM_CONFIG *ofdm_get_config_param(struct OFDM *ofdm);
CODEC2_API int ofdm_get_nin(struct OFDM *);
CODEC2_API int ofdm_get_samples_per_frame(struct OFDM *ofdm);
CODEC2_API int ofdm_get_samples_per_packet(struct OFDM *ofdm);
CODEC2_API int ofdm_get_max_samples_per_frame(struct OFDM *ofdm);
CODEC2_API int ofdm_get_bits_per_frame(struct OFDM *ofdm);
CODEC2_API int ofdm_get_bits_per_packet(struct OFDM *ofdm);
CODEC2_API int ofdm_get_phase_est_bandwidth_mode(struct OFDM *ofdm);

/* option setters */

CODEC2_API void ofdm_set_verbose(struct OFDM *, int);
CODEC2_API void ofdm_set_timing_enable(struct OFDM *, bool);
CODEC2_API void ofdm_set_foff_est_enable(struct OFDM *, bool);
CODEC2_API void ofdm_set_phase_est_enable(struct OFDM *, bool);
CODEC2_API void ofdm_set_phase_est_bandwidth_mode(struct OFDM *ofdm, int val);
CODEC2_API void ofdm_set_off_est_hz(struct OFDM *, float);
CODEC2_API void ofdm_set_sync(struct OFDM *, int);
CODEC2_API void ofdm_set_tx_bpf(struct OFDM *, bool);
CODEC2_API void ofdm_set_dpsk(struct OFDM *ofdm, bool val);
CODEC2_API void ofdm_set_packets_per_burst(struct OFDM *ofdm,
                                            int packetsperburst);

CODEC2_API void ofdm_print_info(struct OFDM *);

#ifdef __cplusplus
}
#endif

#endif
