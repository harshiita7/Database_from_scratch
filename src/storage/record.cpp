#include "storage/record.h"

#include <cstring>
#include <stdexcept>

namespace zeliadb {
namespace {

constexpr std::size_t kSerializedRecordSize = sizeof(std::int32_t) * 3 + kMaxNameLength;

void check_bounds(std::size_t offset) {
    if (offset > kPageSize || kSerializedRecordSize > kPageSize - offset) {
        throw std::out_of_range("record does not fit in page");
    }
}

void copy_bytes(Page& page, std::size_t offset, const void* source, std::size_t size) {
    std::memcpy(page.data() + offset, source, size);
}

void read_bytes(const Page& page, std::size_t offset, void* destination, std::size_t size) {
    std::memcpy(destination, page.data() + offset, size);
}

}  // namespace

void write_student_record(Page& page, std::size_t offset, const StudentRecord& record) {
    check_bounds(offset);
    if (record.name.size() > kMaxNameLength) {
        throw std::invalid_argument("student name is too long");
    }

    const std::int32_t name_length = static_cast<std::int32_t>(record.name.size());
    copy_bytes(page, offset, &record.id, sizeof(record.id));
    copy_bytes(page, offset + sizeof(record.id), &record.age, sizeof(record.age));
    copy_bytes(page, offset + sizeof(record.id) + sizeof(record.age), &name_length, sizeof(name_length));
    copy_bytes(page, offset + sizeof(record.id) * 3, record.name.data(), record.name.size());
}

StudentRecord read_student_record(const Page& page, std::size_t offset) {
    check_bounds(offset);

    StudentRecord record{};
    std::int32_t name_length = 0;
    read_bytes(page, offset, &record.id, sizeof(record.id));
    read_bytes(page, offset + sizeof(record.id), &record.age, sizeof(record.age));
    read_bytes(page, offset + sizeof(record.id) + sizeof(record.age), &name_length, sizeof(name_length));
    if (name_length < 0 || name_length > static_cast<std::int32_t>(kMaxNameLength)) {
        throw std::runtime_error("invalid serialized student name length");
    }

    record.name.resize(static_cast<std::size_t>(name_length));
    if (!record.name.empty()) {
        read_bytes(page, offset + sizeof(record.id) * 3, &record.name[0], record.name.size());
    }
    return record;
}

}  // namespace zeliadb
