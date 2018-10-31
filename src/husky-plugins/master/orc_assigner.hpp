#ifdef WITH_ORC
#pragma once

#include <map>
#include <string>
#include <utility>

#include "hdfs/hdfs.h"
#include "orc/OrcFile.hh"
#include "orc/Reader.hh"

#include "master/master.hpp"

namespace husky {

struct OrcFileDesc {
    std::string filename;
    size_t num_of_rows;
    size_t offset;
};

class ORCBlockAssigner {
   public:
    ORCBlockAssigner();
    ~ORCBlockAssigner() = default;

    void master_main_handler();
    void master_setup_handler();
    /*
    //  void browse_local(const std::string& url);
      /// Return the number of workers who have finished reading the files in
    /// the given url
    int get_num_finished(std::string& url);
    /// Use this when all workers finish reading the files in url
    void finish_url(std::string& url);
   */
    std::pair<std::string, size_t> answer(std::string& url);
    void init_hdfs(const std::string& node, const std::string& port);
    void browse_hdfs(const std::string& url);

   private:
    //  int row_batch_size_ = 8 * 1024;
    //  int num_workers_alive_;

    // handle of HDFS
    hdfsFS fs_;
    int num_workers_alive_;
    //  std::map<std::string, size_t> file_offset_;
    //  std::map<std::string, size_t> file_size_;
    std::map<std::string, std::vector<OrcFileDesc>> files_dict_;
    std::map<std::string, int> finish_dict_;
    std::unique_ptr<orc::Reader> reader_;
};

}  // namespace husky
#endif
