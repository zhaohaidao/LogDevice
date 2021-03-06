/**
 *  Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree.
 **/
#pragma once

#include <folly/futures/Future.h>

#include "logdevice/admin/maintenance/types.h"
#include "logdevice/common/NodeID.h"

namespace facebook { namespace logdevice { namespace maintenance {
/**
 * A SequencerMaintenanceworkflow is a state machine that tracks state
 * transitions of a Sequencer node.
 */
class SequencerWorkflow {
 public:
  explicit SequencerWorkflow(node_index_t node) : node_(node) {}

  // moveable.
  SequencerWorkflow(SequencerWorkflow&& /* unused */) = default;
  SequencerWorkflow& operator=(SequencerWorkflow&& wf) {
    return *this;
  }

  // non-copyable.
  SequencerWorkflow(const SequencerWorkflow& /* unused */) = delete;
  SequencerWorkflow& operator=(const SequencerWorkflow& /* unused */) = delete;

  folly::SemiFuture<MaintenanceStatus> run(bool is_sequencing_enabled);

  // Sets the target_op_state_ to given value
  // Can only be SequencingState::ENABLED or SequencingState::DISABLED
  void setTargetOpState(SequencingState state);

  // Returns the target_op_state_
  SequencingState getTargetOpState() const;

  // Sets skip_safety_check_ to value of `skip`
  void shouldSkipSafetyCheck(bool skip);

  node_index_t getNodeIndex() const {
    return node_;
  }

  virtual ~SequencerWorkflow() {}

 private:
  SequencingState target_op_state_;
  // The shard this workflow is for
  node_index_t node_;
  // True if Sequencing is enabled in NodesConfig.
  // Updated every time run is called
  SequencingState current_sequencing_state_;
  // If true, skip safety check for this workflow
  bool skip_safety_check_{false};
};

}}} // namespace facebook::logdevice::maintenance
