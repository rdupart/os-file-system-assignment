#include <cstdio>
#include <iostream>

struct Student {
    int id;
    char name[50];
    int age;
};

int main() {
    FILE* fp = std::fopen("students.db", "rb");
    if (!fp) {
        std::perror("Failed to open students.db for reading");
        return 1;
    }

    const int recordNumber = 3; // Read only record #3
    long offset = static_cast<long>((recordNumber - 1) * sizeof(Student));

    if (std::fseek(fp, offset, SEEK_SET) != 0) {
        std::perror("fseek failed");
        std::fclose(fp);
        return 1;
    }

    Student s{};
    size_t readCount = std::fread(&s, sizeof(Student), 1, fp);
    if (readCount != 1) {
        std::perror("fread failed for record #3");
        std::fclose(fp);
        return 1;
    }

    std::fclose(fp);

    std::cout << "Record #" << recordNumber << ":\n";
    std::cout << "ID: " << s.id << "\n";
    std::cout << "Name: " << s.name << "\n";
    std::cout << "Age: " << s.age << std::endl;

    return 0;
}
