/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_IN_MEMORY_BLOCK_STORAGE_HPP
#define IROHA_IN_MEMORY_BLOCK_STORAGE_HPP

#include "ametsuchi/block_storage.hpp"

#include <map>

namespace iroha {
  namespace ametsuchi {

    /**
     * Ordered map implementation of block storage
     */
    class InMemoryBlockStorage : public BlockStorage {
     public:
      bool insert(
          Identifier id,
          std::shared_ptr<const shared_model::interface::Block> block) override;

      bool insert(Identifier id,
                  const shared_model::interface::Block &block) override;

      boost::optional<std::shared_ptr<const shared_model::interface::Block>>
      fetch(Identifier id) const override;

      size_t size() const override;

      void clear() override;

      void forEach(FunctionType function) const override;

     private:
      std::map<Identifier,
               std::shared_ptr<const shared_model::interface::Block>>
          block_store_;
    };

  }  // namespace ametsuchi
}  // namespace iroha

#endif  // IROHA_IN_MEMORY_BLOCK_STORAGE_HPP
