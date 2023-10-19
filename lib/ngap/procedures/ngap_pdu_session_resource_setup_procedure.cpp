/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "ngap_pdu_session_resource_setup_procedure.h"
#include "../ngap/ngap_asn1_helpers.h"
#include "ngap_procedure_helpers.h"
#include "srsran/ngap/ngap.h"

using namespace srsran;
using namespace srsran::srs_cu_cp;
using namespace asn1::ngap;

ngap_pdu_session_resource_setup_procedure::ngap_pdu_session_resource_setup_procedure(
    const cu_cp_pdu_session_resource_setup_request& request_,
    byte_buffer                                     nas_pdu_,
    const ngap_ue_ids&                              ue_ids_,
    ngap_rrc_ue_pdu_notifier&                       rrc_ue_pdu_notifier_,
    ngap_du_processor_control_notifier&             du_processor_ctrl_notif_,
    ngap_message_notifier&                          amf_notif_,
    ngap_ue_logger&                                 logger_) :
  request(request_),
  nas_pdu(nas_pdu_),
  ue_ids(ue_ids_),
  rrc_ue_pdu_notifier(rrc_ue_pdu_notifier_),
  du_processor_ctrl_notifier(du_processor_ctrl_notif_),
  amf_notifier(amf_notif_),
  logger(logger_)
{
}

void ngap_pdu_session_resource_setup_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  logger.log_debug("\"{}\" initialized", name());

  // Handle mandatory IEs
  CORO_AWAIT_VALUE(response, du_processor_ctrl_notifier.on_new_pdu_session_resource_setup_request(request));

  // TODO: Handle optional IEs
  if (!nas_pdu.empty()) {
    handle_nas_pdu(logger, std::move(nas_pdu), rrc_ue_pdu_notifier);
  }

  send_pdu_session_resource_setup_response();

  logger.log_debug("\"{}\" finalized", name());

  CORO_RETURN();
}

void ngap_pdu_session_resource_setup_procedure::send_pdu_session_resource_setup_response()
{
  ngap_message ngap_msg = {};

  ngap_msg.pdu.set_successful_outcome();
  ngap_msg.pdu.successful_outcome().load_info_obj(ASN1_NGAP_ID_PDU_SESSION_RES_SETUP);

  fill_asn1_pdu_session_res_setup_response(ngap_msg.pdu.successful_outcome().value.pdu_session_res_setup_resp(),
                                           response);

  auto& pdu_session_res_setup_resp           = ngap_msg.pdu.successful_outcome().value.pdu_session_res_setup_resp();
  pdu_session_res_setup_resp->amf_ue_ngap_id = amf_ue_id_to_uint(ue_ids.amf_ue_id);
  pdu_session_res_setup_resp->ran_ue_ngap_id = ran_ue_id_to_uint(ue_ids.ran_ue_id);

  logger.log_info("Sending PduSessionResourceSetupResponse");
  amf_notifier.on_new_message(ngap_msg);
}
