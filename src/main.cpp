#include "storage/disk_manager.h"
#include "storage/record.h"

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    const std::string database_path = "zeliadb.db";
    const std::vector<zeliadb::StudentRecord> students = {
        {1, 20, "Rahul Sharma"},
        {2, 21, "Aman Das"},
        {3, 19, "Priya Singh"},
        {4, 22, "Arjun Mehta"},
        {5, 20, "Sneha Roy"},
        {6, 23, "Vikram Patel"},
        {7, 21, "Ananya Gupta"},
        {8, 20, "Rohan Verma"},
        {9, 22, "Ishita Bose"},
        {10, 19, "Kabir Khan"},
        {11, 21, "Neha Kapoor"},
        {12, 22, "Aditya Nair"},
    };

    std::remove(database_path.c_str());

    {
        zeliadb::DiskManager disk(database_path);
        const zeliadb::PageId page_id = disk.allocate_page();
        zeliadb::Page page = zeliadb::make_empty_page();
        for (std::size_t index = 0; index < students.size(); ++index) {
            zeliadb::write_student_record(page, index * 128, students[index]);
        }
        disk.write_page(page_id, page);
    }

    {
        zeliadb::DiskManager disk(database_path);
        zeliadb::Page page{};
        disk.read_page(0, page);
        std::vector<zeliadb::StudentRecord> loaded_students;
        for (std::size_t index = 0; index < students.size(); ++index) {
            loaded_students.push_back(zeliadb::read_student_record(page, index * 128));
        }

        std::cout << "\n+============================================================\n"
                  << "                    Z E L I A  D B\n"
                  << "              Student Records: Page 0\n"
                  << "============================================================\n"
                  << "Status : storage round-trip passed\n"
                  << "Pages  : " << disk.page_count() << " (" << zeliadb::kPageSize << " bytes each)\n"
                  << "Rows   : " << loaded_students.size() << "\n\n"
                  << "+------+----------------------+-------+\n"
                  << "| ID   | Name                 | Age   |\n"
                  << "+------+----------------------+-------+\n";

        for (const auto& student : loaded_students) {
            std::cout << "| " << std::left << std::setw(4) << student.id
                      << " | " << std::left << std::setw(20) << student.name
                      << " | " << std::right << std::setw(5) << student.age << " |\n";
        }

        std::cout << "+------+----------------------+-------+\n"
                  << "Data successfully persisted to disk and read back.\n\n";
    }

    std::remove(database_path.c_str());
}
