#pragma once

#include "storage/page.h"

#include <cstdint>
#include <fstream>
#include <string>

namespace zeliadb {

class DiskManager {
public:
    explicit DiskManager(const std::string& database_path);
    ~DiskManager();

    DiskManager(const DiskManager&) = delete;
    DiskManager& operator=(const DiskManager&) = delete;

    PageId allocate_page();
    void read_page(PageId page_id, Page& page);
    void write_page(PageId page_id, const Page& page);
    std::uint32_t page_count();

private:
    void ensure_open();

    std::string database_path_;
    std::fstream file_;
};

}  // namespace zeliadb
