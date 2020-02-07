/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TRANSACTIONMQPRODUCER_H__
#define __TRANSACTIONMQPRODUCER_H__

#include <memory>
#include <string>
#include "MQClient.h"
#include "MQMessage.h"
#include "MQMessageExt.h"
#include "SessionCredentials.h"
#include "TransactionListener.h"
#include "TransactionSendResult.h"

namespace rocketmq {
class TransactionMQProducerImpl;
class ROCKETMQCLIENT_API TransactionMQProducer {
 public:
  TransactionMQProducer(const std::string& producerGroup);
  virtual ~TransactionMQProducer();
  void start();
  void shutdown();
  const std::string& getNamesrvAddr() const;
  void setNamesrvAddr(const std::string& namesrvAddr);
  const std::string& getNamesrvDomain() const;
  void setNamesrvDomain(const std::string& namesrvDomain);
  const std::string& getInstanceName() const;
  void setInstanceName(const std::string& instanceName);
  // nameSpace
  const std::string& getNameSpace() const;
  void setNameSpace(const std::string& nameSpace);
  const std::string& getGroupName() const;
  void setGroupName(const std::string& groupname);
  void setSessionCredentials(const std::string& accessKey,
                             const std::string& secretKey,
                             const std::string& accessChannel);
  const SessionCredentials& getSessionCredentials() const;

  void setUnitName(std::string unitName);
  const std::string& getUnitName() const;

  int getSendMsgTimeout() const;
  void setSendMsgTimeout(int sendMsgTimeout);
  void setTcpTransportPullThreadNum(int num);
  const int getTcpTransportPullThreadNum() const;

  /** timeout of tcp connect, it is same meaning for both producer and consumer;
   *    1. default value is 3000ms
   *    2. input parameter could only be milliSecond, suggestion value is
   *       1000-3000ms;
   **/
  void setTcpTransportConnectTimeout(uint64_t timeout);  // ms
  const uint64_t getTcpTransportConnectTimeout() const;

  /** timeout of tryLock tcpTransport before sendMsg/pullMsg, if timeout,
   *  returns NULL
   *    1. paremeter unit is ms, default value is 3000ms, the minimun value is 1000ms
   *       suggestion value is 3000ms;
   *    2. if configured with value smaller than 1000ms, the tryLockTimeout value
   *       will be setted to 1000ms
   **/
  void setTcpTransportTryLockTimeout(uint64_t timeout);  // ms
  const uint64_t getTcpTransportTryLockTimeout() const;

  /*
   *  if msgBody size is large than m_compressMsgBodyOverHowmuch
   *  rocketmq cpp will compress msgBody according to compressLevel
   */
  int getCompressMsgBodyOverHowmuch() const;
  void setCompressMsgBodyOverHowmuch(int compressMsgBodyOverHowmuch);
  int getCompressLevel() const;
  void setCompressLevel(int compressLevel);

  // if msgbody size larger than maxMsgBodySize, exception will be throwed
  int getMaxMessageSize() const;
  void setMaxMessageSize(int maxMessageSize);
  // log configuration interface, default LOG_LEVEL is LOG_LEVEL_INFO, default
  // log file num is 3, each log size is 100M
  void setLogLevel(elogLevel inputLevel);
  elogLevel getLogLevel();
  void setLogFileSizeAndNum(int fileNum, long perFileSize);  // perFileSize is MB unit

  std::shared_ptr<TransactionListener> getTransactionListener();
  void setTransactionListener(TransactionListener* listener);
  TransactionSendResult sendMessageInTransaction(MQMessage& msg, void* arg);
  void checkTransactionState(const std::string& addr,
                             const MQMessageExt& message,
                             long tranStateTableOffset,
                             long commitLogOffset,
                             const std::string& msgId,
                             const std::string& transactionId,
                             const std::string& offsetMsgId);

 private:
  TransactionMQProducerImpl* impl;
};
}  // namespace rocketmq

#endif
