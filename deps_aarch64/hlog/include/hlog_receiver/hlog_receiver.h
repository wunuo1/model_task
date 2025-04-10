// Copyright (c) [2021-2023] [Horizon Robotics].
//
// You can use this software according to the terms and conditions of
// the Apache v2.0.
// You may obtain a copy of Apache v2.0. at:
//
//     http: //www.apache.org/licenses/LICENSE-2.0
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
// See Apache v2.0 for more details.

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "visibility.h"

namespace hobot {
namespace hlog {
class ZmqReceiveProto;
namespace hlog_receiver {

class HlogReceiverImpl;


/**
 * @brief use this class to receive log sent through internet
 *
 */
class HLRECEIVER_PUBLIC HlogReceiver {
 public:
  using ReceiveCallback = std::function<void(std::string)>;

  /**
   * @brief Construct a new Hlog Receiver object
   *
   * @param listener[in] user callback function to get recevied log
   * @param log_source_id[in] the identifier of the interested log source, which
   * is organized in for of ip:pid of the log source. The ip of 0.0.0.0 means
   * all source ip, pid of 0 means all source pid. Empty means all log source.
   * @param module_name[in] the interested module of log. Empty means all
   * modules.
   * @param host_ip the ip of host, if empty, hlog receiver will chose the first
   * ip founded of all ip addresses of host, this ip may not able to communicate
   * with the one you wanted, in this case, you should designate the host ip by
   * yourself.
   */
  explicit HlogReceiver(const ReceiveCallback &listener,
                        const std::string &log_source_id = {},
                        const std::string &module_name = {},
                        const std::string &host_ip = {});

  /**
   * @brief Destroy the Hlog Receiver object
   *
   */
  ~HlogReceiver();

  /**
   * @brief start receiver
   *
   * @return true start success
   * @return false start fail
   */
  bool StartReceiver();

  /**
   * @brief stop receiver
   *
   */
  void StopReceiver();

 private:
  std::unique_ptr<HlogReceiverImpl> impl_;
};


class HLRECEIVER_PUBLIC HlogTcpReceiver {
 public:
  using ReceiveCallback = std::function<void(const std::string&)>;
  using EventCallback = std::function<void(bool is_connected)>;
  HlogTcpReceiver(const ReceiveCallback &listener,
                  const std::string &log_source_id,
                  const uint32_t log_source_port,
                  const bool port_is_pid = true,
                  const EventCallback &event_cb = nullptr);
  ~HlogTcpReceiver();
  bool StartReceiver();
  bool StopReceiver();
 private:
  std::shared_ptr<ZmqReceiveProto> zmq_recv_proto_{nullptr};
};

}  // namespace hlog_receiver
}  // namespace hlog
}  // namespace hobot
