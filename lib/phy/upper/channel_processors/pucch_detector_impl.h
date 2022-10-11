/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

/// \file
/// \brief PUCCH detector declaration.

#pragma once

#include "srsgnb/phy/upper/channel_processors/pucch_detector.h"
#include "srsgnb/phy/upper/sequence_generators/low_papr_sequence_collection.h"
#include "srsgnb/phy/upper/sequence_generators/pseudo_random_generator.h"

namespace srsgnb {

/// PUCCH detector implementation for Formats 0 and 1.
class pucch_detector_impl : public pucch_detector
{
public:
  /// \brief Maximum number of REs allocated to PUCCH Format 1.
  ///
  /// The allocated resources are at most one PRB over all OFDM symbols.
  static constexpr unsigned MAX_ALLOCATED_RE_F1 = NRE * MAX_NSYMB_PER_SLOT;

  /// Maximum number of OFDM symbols allocated for PUCCH Format 1 data.
  static constexpr unsigned MAX_N_DATA_SYMBOLS = 7;

  /// \brief Constructor: provides access to a collection of low-PAPR sequences and a pseudorandom sequence generator.
  /// \param[in] lpsc Pointer to a collection of low-PAPR sequences.
  /// \param[in] prg  Pointer to a pseudorandom sequence generator.
  /// \remark The low-PAPR collection should correspond to the cyclic shifts \f$\{\alpha : \alpha = 2 \pi
  /// \frac{n}{N_{\textup{sc}}^{\textup{RB}}}, \quad n = 0, \dots, N_{\textup{sc}}^{\textup{RB}}-1\}\f$, where
  /// \f$N_{\textup{sc}}^{\textup{RB}} = 12\f$ is the number of subcarriers in a resource block.
  pucch_detector_impl(std::unique_ptr<low_papr_sequence_collection> lpsc,
                      std::unique_ptr<pseudo_random_generator>      prg) :
    low_papr(std::move(lpsc)), pseudo_random(std::move(prg))
  {
  }

  // See interface for documentation.
  std::pair<pucch_uci_message, channel_state_information> detect(const resource_grid_reader& /*grid*/,
                                                                 const format0_configuration& /*config*/) override
  {
    srsgnb_assert(false, "PUCCH Format 0 not implemented yet.");
    return {};
  }

  // See interface for documentation.
  pucch_uci_message detect(const resource_grid_reader&  grid,
                           const channel_estimate&      estimates,
                           const format1_configuration& config) override;

private:
  /// \brief Extracts PUCCH data and channel coefficients.
  ///
  /// This method extracts the PUCCH data REs from the resource grid as well as the corresponding channel estimates.
  void extract_data_and_estimates(const resource_grid_reader& grid,
                                  const channel_estimate&     estimates,
                                  unsigned                    first_symbol,
                                  unsigned                    first_prb,
                                  const optional<unsigned>&   second_prb,
                                  unsigned                    port);

  /// \brief Marginalizes the spreading sequences out.
  ///
  /// A PUCCH Format 1 consists of a single modulation symbol spread over all time and frequency allocated resources.
  /// This method combines all the replicas into a single estimate of the modulation symbol and it computes the
  /// equivalent noise variance. The PUCCH configuration is needed to build the proper spreading sequences.
  void marginalize_w_and_r_out(const format1_configuration& config);

  /// Collection of low-PAPR sequences.
  std::unique_ptr<low_papr_sequence_collection> low_papr;
  /// Pseudorandom sequence generator.
  std::unique_ptr<pseudo_random_generator> pseudo_random;
  /// \brief Buffer for storing the spread data sequence.
  /// \remark Only half of the allocated symbols contain data, the other half being used for DM-RS.
  static_vector<cf_t, MAX_ALLOCATED_RE_F1 / 2> time_spread_sequence;
  /// Buffer for storing the channel estimates corresponding to the spread data sequence.
  static_vector<cf_t, MAX_ALLOCATED_RE_F1 / 2> ch_estimates;
  /// \brief Buffer for storing the spread data sequence after equalization.
  /// \remark Only half of the allocated symbols contain data, the other half being used for DM-RS.
  static_vector<cf_t, MAX_ALLOCATED_RE_F1 / 2> eq_time_spread_sequence;
  /// Buffer for storing the equivalent channel noise variances corresponding to the equalized spread data sequence.
  static_vector<float, MAX_ALLOCATED_RE_F1 / 2> eq_time_spread_noise_var;
  /// Buffer for storing the data symbol spread over all allocated resources.
  static_vector<cf_t, MAX_ALLOCATED_RE_F1 / 2> repeated_symbol;
  /// Detected symbol.
  cf_t detected_symbol = 0;
  /// Equivalent noise variance of detected symbol.
  float eq_noise_var = 0;
  /// Total number of data symbols.
  unsigned nof_data_symbols = 0;
  /// Number of data symbols before frequency hopping.
  unsigned nof_data_symbols_pre_hop = 0;
};

} // namespace srsgnb