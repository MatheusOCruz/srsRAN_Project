/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "../ngap_asn1_utils.h"
#include "../ue_context/ngap_ue_context.h"
#include "srsran/ngap/ngap.h"
#include "srsran/ngap/ngap_types.h"
#include "srsran/support/async/async_task.h"

namespace srsran {
namespace srs_cu_cp {

class ngap_pdu_session_resource_modify_procedure
{
public:
  ngap_pdu_session_resource_modify_procedure(const cu_cp_pdu_session_resource_modify_request& request_,
                                             const ngap_ue_ids&                               ue_ids_,
                                             ngap_du_processor_control_notifier&              du_processor_ctrl_notif_,
                                             ngap_message_notifier&                           amf_notif_,
                                             ngap_ue_logger&                                  logger_);

  void operator()(coro_context<async_task<void>>& ctx);

  static const char* name() { return "PDU Session Resource Modify Procedure"; }

private:
  // results senders
  void send_pdu_session_resource_modify_response();

  cu_cp_pdu_session_resource_modify_request  request;
  const ngap_ue_ids                          ue_ids;
  cu_cp_pdu_session_resource_modify_response response;
  ngap_du_processor_control_notifier&        du_processor_ctrl_notifier;
  ngap_message_notifier&                     amf_notifier;
  ngap_ue_logger&                            logger;
};

} // namespace srs_cu_cp
} // namespace srsran
