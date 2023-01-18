/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

#include "e2.h"
#include "srsgnb/support/timers.h"
#include <memory>

namespace srsgnb {

/// Creates an instance of an E2 interface, notifying outgoing packets on the specified listener object.
std::unique_ptr<e2_interface> create_e2(srsgnb::timer_manager& timers_, e2_message_notifier& e2_pdu_notifier_);

} // namespace srsgnb