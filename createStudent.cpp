#include <cstdio>
#include <cstring>
#include <iostream>

using namespace std;

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

    FILE* fp = fopen("students.db", "wb");
    if (!fp) {
        perror("Failed to open students.db for writing");
        return 1;
    }

    size_t written = fwrite(students, sizeof(Student), 5, fp);
    if (written != 5) {
        perror("Failed to write all student records");
        fclose(fp);
        return 1;
    }

    fclose(fp);
    cout << "students.db created with 5 records." << endl;

    return 0;
}
