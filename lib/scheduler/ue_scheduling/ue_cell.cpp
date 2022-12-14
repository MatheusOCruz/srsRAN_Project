/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "ue_cell.h"
#include "../support/dmrs_helpers.h"
#include "../support/prbs_calculator.h"

using namespace srsgnb;

ue_cell::ue_cell(du_ue_index_t                     ue_index_,
                 rnti_t                            crnti_val,
                 const scheduler_ue_expert_config& expert_cfg_,
                 const cell_configuration&         cell_cfg_common_,
                 const serving_cell_config&        ue_serv_cell) :
  ue_index(ue_index_),
  cell_index(ue_serv_cell.cell_index),
  harqs(crnti_val, 8),
  crnti_(crnti_val),
  expert_cfg(expert_cfg_),
  ue_cfg(cell_cfg_common_, ue_serv_cell)
{
}

void ue_cell::handle_reconfiguration_request(const serving_cell_config& new_ue_cell_cfg)
{
  ue_cfg.reconfigure(new_ue_cell_cfg);
}

unsigned ue_cell::required_dl_prbs(unsigned time_resource, unsigned pending_bytes) const
{
  static const unsigned     nof_layers = 1;
  const cell_configuration& cell_cfg   = cfg().cell_cfg_common;

  sch_mcs_index       mcs        = expert_cfg.fixed_dl_mcs.value(); // TODO: Support dynamic MCS.
  sch_mcs_description mcs_config = pdsch_mcs_get_config(cfg().cfg_dedicated().init_dl_bwp.pdsch_cfg->mcs_table, mcs);

  dmrs_information dmrs_info = make_dmrs_info_common(
      cell_cfg.dl_cfg_common.init_dl_bwp.pdsch_common, time_resource, cell_cfg.pci, cell_cfg.dmrs_typeA_pos);
  // According to TS 38.214, Section 5.1.3.2, nof_oh_prb is set equal to xOverhead, when set; else nof_oh_prb = 0.
  // NOTE: x_overhead::not_set is mapped to 0.
  unsigned     nof_oh_prb = cfg().cfg_dedicated().pdsch_serv_cell_cfg.has_value()
                                ? static_cast<unsigned>(cfg().cfg_dedicated().pdsch_serv_cell_cfg.value().x_ov_head)
                                : static_cast<unsigned>(x_overhead::not_set);
  sch_prbs_tbs prbs_tbs   = get_nof_prbs(prbs_calculator_sch_config{
      pending_bytes,
      (unsigned)cell_cfg.dl_cfg_common.init_dl_bwp.pdsch_common.pdsch_td_alloc_list[time_resource].symbols.length(),
      calculate_nof_dmrs_per_rb(dmrs_info),
      nof_oh_prb,
      mcs_config,
      nof_layers});
  return prbs_tbs.nof_prbs;
}

unsigned ue_cell::required_ul_prbs(unsigned time_resource, unsigned pending_bytes) const
{
  static const unsigned     nof_layers = 1;
  const cell_configuration& cell_cfg   = cfg().cell_cfg_common;

  sch_mcs_index       mcs = expert_cfg.fixed_ul_mcs.value(); // TODO: Support dynamic MCS.
  sch_mcs_description mcs_config =
      pusch_mcs_get_config(cfg().cfg_dedicated().ul_config->init_ul_bwp.pusch_cfg->mcs_table, mcs, false);

  dmrs_information dmrs_info = make_dmrs_info_common(
      *cell_cfg.ul_cfg_common.init_ul_bwp.pusch_cfg_common, time_resource, cell_cfg.pci, cell_cfg.dmrs_typeA_pos);
  // According to TS 38.214, Section 6.1.4.2, nof_oh_prb is set equal to xOverhead, when set; else nof_oh_prb = 0.
  // NOTE: x_overhead::not_set is mapped to 0.
  unsigned     nof_oh_prb = cfg().cfg_dedicated().pdsch_serv_cell_cfg.has_value()
                                ? static_cast<unsigned>(cfg().cfg_dedicated().pdsch_serv_cell_cfg.value().x_ov_head)
                                : static_cast<unsigned>(x_overhead::not_set);
  sch_prbs_tbs prbs_tbs   = get_nof_prbs(prbs_calculator_sch_config{
      pending_bytes,
      (unsigned)cell_cfg.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list[time_resource]
          .symbols.length(),
      calculate_nof_dmrs_per_rb(dmrs_info),
      nof_oh_prb,
      mcs_config,
      nof_layers});
  return prbs_tbs.nof_prbs;
}
