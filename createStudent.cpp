#include <cstdio>
#include <cstring>
#include <iostream>

struct Student {
    int id;
    char name[50];
    int age;
};

int main() {
    Student students[5] = {
        {101, "Alice", 20},
        {102, "Bob", 21},
        {103, "Charlie", 22},
        {104, "Diana", 20},
        {105, "Ethan", 23}
    };

    FILE* fp = std::fopen("students.db", "wb");
    if (!fp) {
        std::perror("Failed to open students.db for writing");
        return 1;
    }

    size_t written = std::fwrite(students, sizeof(Student), 5, fp);
    if (written != 5) {
        std::perror("Failed to write all student records");
        std::fclose(fp);
        return 1;
    }

    std::fclose(fp);
    std::cout << "students.db created with 5 records." << std::endl;
    return 0;
}
