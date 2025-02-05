/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#ifndef PULSAR_CPP_LOOKUPSERVICE_H
#define PULSAR_CPP_LOOKUPSERVICE_H

#include <pulsar/Result.h>
#include <pulsar/Schema.h>

#include <memory>
#include <ostream>
#include <vector>

#include "Future.h"
#include "LookupDataResult.h"
#include "ProtoApiEnums.h"

namespace pulsar {
using NamespaceTopicsPtr = std::shared_ptr<std::vector<std::string>>;
class TopicName;
using TopicNamePtr = std::shared_ptr<TopicName>;
class NamespaceName;
using NamespaceNamePtr = std::shared_ptr<NamespaceName>;

class LookupService {
   public:
    struct LookupResult {
        std::string logicalAddress;
        std::string physicalAddress;

        friend std::ostream& operator<<(std::ostream& os, const LookupResult& lookupResult) {
            return os << "logical address: " << lookupResult.logicalAddress
                      << ", physical address: " << lookupResult.physicalAddress;
        }
    };
    using LookupResultFuture = Future<Result, LookupResult>;
    using LookupResultPromise = Promise<Result, LookupResult>;

    /**
     * Call broker lookup-api to get broker which serves namespace bundle that contains the given topic.
     *
     * @param topicName the topic name
     * @return a pair of addresses, representing the logical and physical addresses of the broker that serves
     * the topic
     */
    virtual LookupResultFuture getBroker(const TopicName& topicName) = 0;

    /*
     * @param    topicName - pointer to topic name
     *
     * Gets Partition metadata
     */
    virtual Future<Result, LookupDataResultPtr> getPartitionMetadataAsync(const TopicNamePtr& topicName) = 0;

    /**
     * @param   namespace - namespace-name
     *
     * Returns all the topics name for a given namespace.
     */
    virtual Future<Result, NamespaceTopicsPtr> getTopicsOfNamespaceAsync(
        const NamespaceNamePtr& nsName, CommandGetTopicsOfNamespace_Mode mode) = 0;

    /**
     * Get the SchemaInfo for a given topic and a specific schema version.
     *
     * @param topicName topic-name
     * @param version the schema version byte array, if it's empty, use the latest version
     * @return SchemaInfo
     */
    virtual Future<Result, SchemaInfo> getSchema(const TopicNamePtr& topicName,
                                                 const std::string& version = "") = 0;

    virtual ~LookupService() {}

    virtual void close() {}
};

typedef std::shared_ptr<LookupService> LookupServicePtr;

}  // namespace pulsar
#endif  // PULSAR_CPP_LOOKUPSERVICE_H
