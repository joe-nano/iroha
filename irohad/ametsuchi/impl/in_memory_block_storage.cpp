/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ametsuchi/impl/in_memory_block_storage.hpp"

using namespace iroha::ametsuchi;

bool InMemoryBlockStorage::insert(
    Identifier id,
    std::shared_ptr<const shared_model::interface::Block> block) {
  return block_store_.emplace(id, std::move(block)).second;
}

bool InMemoryBlockStorage::insert(Identifier id,
                                  const shared_model::interface::Block &block) {
  return block_store_.emplace(id, clone(block)).second;
}

boost::optional<std::shared_ptr<const shared_model::interface::Block>>
InMemoryBlockStorage::fetch(Identifier id) const {
  auto it = block_store_.find(id);
  if (it != block_store_.end()) {
    return it->second;
  } else {
    return boost::none;
  }
}

size_t InMemoryBlockStorage::size() const {
  return block_store_.size();
}

void InMemoryBlockStorage::clear() {
  block_store_.clear();
}

void InMemoryBlockStorage::forEach(FunctionType function) const {
  for (const auto &pair : block_store_) {
    function(pair.first, pair.second);
  }
}
