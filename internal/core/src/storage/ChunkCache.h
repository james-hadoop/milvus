// Licensed to the LF AI & Data foundation under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership. The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <oneapi/tbb/concurrent_hash_map.h>
#include "mmap/Column.h"

namespace milvus::storage {

class ChunkCache {
 public:
    explicit ChunkCache(const std::string& path, ChunkManagerPtr cm)
        : path_prefix_(path), cm_(cm) {
        LOG_SEGCORE_INFO_ << "Init ChunkCache with prefix: " << path_prefix_;
    }

    ~ChunkCache() = default;

 public:
    std::shared_ptr<ColumnBase>
    Read(const std::string& filepath);

    void
    Remove(const std::string& filepath);

    void
    Prefetch(const std::string& filepath);

 private:
    std::shared_ptr<ColumnBase>
    Mmap(const std::filesystem::path& path, const FieldDataPtr& field_data);

 private:
    using ColumnTable =
        oneapi::tbb::concurrent_hash_map<std::string,
                                         std::shared_ptr<ColumnBase>>;

 private:
    mutable std::mutex mutex_;
    std::string path_prefix_;
    ChunkManagerPtr cm_;
    ColumnTable columns_;
};

using ChunkCachePtr = std::shared_ptr<milvus::storage::ChunkCache>;

}  // namespace milvus::storage
