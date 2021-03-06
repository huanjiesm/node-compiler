// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_INTERPRETER_BYTECODE_PEEPHOLE_OPTIMIZER_H_
#define V8_INTERPRETER_BYTECODE_PEEPHOLE_OPTIMIZER_H_

#include "src/interpreter/bytecode-peephole-table.h"
#include "src/interpreter/bytecode-pipeline.h"

namespace v8 {
namespace internal {
namespace interpreter {

class BytecodePeepholeActionAndData;

// An optimization stage for performing peephole optimizations on
// generated bytecode. The optimizer may buffer one bytecode
// internally.
class BytecodePeepholeOptimizer final : public BytecodePipelineStage,
                                        public ZoneObject {
 public:
  explicit BytecodePeepholeOptimizer(BytecodePipelineStage* next_stage);

  // BytecodePipelineStage interface.
  void Write(BytecodeNode* node) override;
  void WriteJump(BytecodeNode* node, BytecodeLabel* label) override;
  void BindLabel(BytecodeLabel* label) override;
  void BindLabel(const BytecodeLabel& target, BytecodeLabel* label) override;
  Handle<BytecodeArray> ToBytecodeArray(
      Isolate* isolate, int register_count, int parameter_count,
      Handle<FixedArray> handler_table) override;

 private:
#define DECLARE_ACTION(Action)          \
  void Action(BytecodeNode* const node, \
              const PeepholeActionAndData* const action_data = nullptr);
  PEEPHOLE_ACTION_LIST(DECLARE_ACTION)
#undef DECLARE_ACTION

  void ApplyPeepholeAction(BytecodeNode* const node);
  void Flush();
  bool CanElideLastBasedOnSourcePosition(
      const BytecodeNode* const current) const;
  void InvalidateLast();
  bool LastIsValid() const;
  void SetLast(const BytecodeNode* const node);

  BytecodePipelineStage* next_stage() const { return next_stage_; }
  BytecodeNode* last() { return &last_; }

  BytecodePipelineStage* next_stage_;
  BytecodeNode last_;

  DISALLOW_COPY_AND_ASSIGN(BytecodePeepholeOptimizer);
};

}  // namespace interpreter
}  // namespace internal
}  // namespace v8

#endif  // V8_INTERPRETER_BYTECODE_PEEPHOLE_OPTIMIZER_H_
