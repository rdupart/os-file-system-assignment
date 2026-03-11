#include <cstdio>
#include <iostream>

using namespace std;

struct Student {
    int id;
    char name[50];
    int age;
};

int main() {
    FILE* fp = fopen("students.db", "rb");
    if (!fp) {
        perror("Failed to open students.db for reading");
        return 1;
    }

    const int recordNumber = 3; // Read only record #3
    long offset = static_cast<long>((recordNumber - 1) * sizeof(Student));

    if (fseek(fp, offset, SEEK_SET) != 0) {
        perror("fseek failed");
        fclose(fp);
        return 1;
    }

    Student s{};
    size_t readCount = fread(&s, sizeof(Student), 1, fp);
    if (readCount != 1) {
        perror("fread failed for record #3");
        fclose(fp);
        return 1;
    }

    fclose(fp);

    cout << "Record #" << recordNumber << ":\n";
    cout << "ID: " << s.id << "\n";
    cout << "Name: " << s.name << "\n";
    cout << "Age: " << s.age << endl;

    return 0;
}
