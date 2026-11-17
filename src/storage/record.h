#pragma once

#include "storage/page.h"

#include <cstdint>
#include <string>

namespace zeliadb {

struct StudentRecord {
    std::int32_t id;
    std::int32_t age;
    std::string name;
};

constexpr std::size_t kMaxNameLength = 100;

void write_student_record(Page& page, std::size_t offset, const StudentRecord& record);
StudentRecord read_student_record(const Page& page, std::size_t offset);

}  // namespace zeliadb
