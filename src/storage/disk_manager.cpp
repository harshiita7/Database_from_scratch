#include "storage/disk_manager.h"

#include <stdexcept>

namespace zeliadb {

DiskManager::DiskManager(const std::string& database_path)
    : database_path_(database_path) {
    file_.open(database_path_, std::ios::in | std::ios::out | std::ios::binary);
    if (!file_) {
        file_.clear();
        std::ofstream create_file(database_path_, std::ios::binary);
        create_file.close();
        file_.open(database_path_, std::ios::in | std::ios::out | std::ios::binary);
    }
    ensure_open();
}

DiskManager::~DiskManager() {
    if (file_.is_open()) {
        file_.flush();
        file_.close();
    }
}

PageId DiskManager::allocate_page() {
    const PageId page_id = page_count();
    const Page empty_page = make_empty_page();
    write_page(page_id, empty_page);
    return page_id;
}

void DiskManager::read_page(PageId page_id, Page& page) {
    ensure_open();
    if (page_id >= page_count()) {
        throw std::out_of_range("page id is outside the database file");
    }

    file_.clear();
    file_.seekg(static_cast<std::streamoff>(page_id) * kPageSize);
    file_.read(reinterpret_cast<char*>(page.data()), static_cast<std::streamsize>(kPageSize));
    if (file_.gcount() != static_cast<std::streamsize>(kPageSize)) {
        throw std::runtime_error("could not read a complete page");
    }
}

void DiskManager::write_page(PageId page_id, const Page& page) {
    ensure_open();
    file_.clear();
    file_.seekp(static_cast<std::streamoff>(page_id) * kPageSize);
    file_.write(reinterpret_cast<const char*>(page.data()), static_cast<std::streamsize>(kPageSize));
    file_.flush();
    if (!file_) {
        throw std::runtime_error("could not write page");
    }
}

std::uint32_t DiskManager::page_count() {
    if (!file_.is_open()) {
        throw std::runtime_error("database file is closed");
    }

    const auto current_position = file_.tellg();
    file_.clear();
    file_.seekg(0, std::ios::end);
    const auto bytes = file_.tellg();
    file_.seekg(current_position);
    return static_cast<std::uint32_t>(bytes / static_cast<std::streamoff>(kPageSize));
}

void DiskManager::ensure_open() {
    if (!file_.is_open()) {
        throw std::runtime_error("could not open database file: " + database_path_);
    }
}

}  // namespace zeliadb
