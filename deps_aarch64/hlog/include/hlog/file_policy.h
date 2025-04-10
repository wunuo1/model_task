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

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace hobot {
namespace hlog {

using FileNamePair = std::pair<std::string, std::string>;

class FilePolicy {
 public:
  explicit FilePolicy(const std::string& name) : name_(name) {}
  virtual ~FilePolicy() = default;

  const std::string& GetName(void) { return name_; }

  void SetFileHint(const std::string& hint) { file_hint_ = hint; }
  void SetMaxRotateNumber(int max_rotate_number) {
    max_rotate_num_ = max_rotate_number;
  }

  void SetMaxStoreSize(uint64_t max_store_size) {
    max_store_size_ = max_store_size;
  }

  /// @brief it is called, to test if the log file is accessable
  /// if the passed hint cannot satisify the requirements of the policy
  /// return empyt std::string to indicated something wrong
  ///
  virtual std::string GetLogFileNameForTest(const std::string& hint) = 0;

  /// all the functions below are called in single thread and single process
  /// context, protected by locks

  /// @brief  generate the lock file name to protect from multiple
  ///         processes access

  virtual std::string GetLockFileName(void) = 0;

  /// @brief on each time open the log file, this function is called
  ///       to get log file name. This enables the log file changed on every
  ///       open
  virtual std::string GetLogFileName(void) = 0;

  /// @brief return the filename list matches with file naming policy with newer
  /// first
  ///     it is called when the sink is enabled
  virtual std::vector<std::string> GetMatchedFileList(void) = 0;

  /// the following fours API will be called during log file rotate

  /// @brief return the rotate pair list, the newer pair first
  virtual std::vector<FileNamePair> GetRotateFileList(void) = 0;

  /// @brief return the file list to be removed when do rotating
  virtual std::vector<std::string> GetRemovedFileList(void) = 0;

  /// @brief return the directory list to be created before do rotating
  virtual std::vector<std::string> GetCreateDirList(void) { return {}; }

  /// @brief return the directory list to be removed after rotating done
  virtual std::vector<std::string> GetRemovedDirList(void) { return {}; }

  /// @brief the notify interfaces
  virtual bool OnConfigCheck(void) = 0;

  virtual bool OnSinkEnable(void) { return true; }

  virtual void OnSinkDisable(void) {}
  virtual void OnRotateStart(void) {}
  virtual void OnRotateEnd(void) {}

 protected:
  int max_rotate_num_{1};
  uint64_t max_store_size_{0};
  std::string file_hint_;

 private:
  std::string name_;
};

using FilePolicyPtr = std::shared_ptr<FilePolicy>;

}  // namespace hlog
}  // namespace hobot
