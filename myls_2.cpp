#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <grp.h>
//#include <stdlib.h> // For free()
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>

using namespace std;

// function to sort an array of string of some size.
void sort(string n[], int size) {
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if (n[j] > n[j + 1]) {
				string tmp = n[j];
				n[j] = n[j + 1];
				n[j + 1] = tmp;
			}
		}
	}
}

// functino to get permission from mode
string formatPermissions(mode_t mode) {
	string perms(10, '-');

	if (S_ISDIR(mode)) perms[0] = 'd';
	else if (S_ISLNK(mode)) perms[0] = 'l';

	if (mode & S_IRUSR) perms[1] = 'r';
	if (mode & S_IWUSR) perms[2] = 'w';
	if (mode & S_IXUSR) perms[3] = 'x';
	if (mode & S_IRGRP) perms[4] = 'r';
	if (mode & S_IWGRP) perms[5] = 'w';
	if (mode & S_IXGRP) perms[6] = 'x';
	if (mode & S_IROTH) perms[7] = 'r';
	if (mode & S_IWOTH) perms[8] = 'w';
	if (mode & S_IXOTH) perms[9] = 'x';

	return perms;
}

// finction to return "[DIR]     ", ["SYMLNK]", "[FILE]    ", "[OTHER]  "
// based on the mode
string fileType(mode_t mode) {
	if (S_ISDIR(mode)) return "[DIR]     ";
	if (S_ISLNK(mode)) return "[SYMLNK]  ";
	if (S_ISREG(mode)) return "[FILE]    ";
	return "[OTHER]   ";
}

// function to format time as [Month name] [day] [hour]:[min]
string fomratTime(time_t t) {
	char buf[64];
	struct tm *tm_info = localtime(&t);
	if (!tm_info) return "N/A";
	strftime(buf, sizeof(buf), "%b %d %H:%M", tm_info);
	return string(buf);
}

int main() {
	char cwd[256];
	string names[256];
	string user_cmd;
	while (1) {
		getcwd(cwd, sizeof(cwd));
		cout << endl << cwd << "> ";

		getline(cin, user_cmd);
		if (user_cmd == "exit") break;

		if (user_cmd.substr(0, 2) == "ls") {
			bool showAll = false;
			bool longFormat = false;

			if (user_cmd == "ls") {
				// default
			} else if (user_cmd == "ls -l") {
				longFormat = true;
			} else if (user_cmd == "ls -a") {
				showAll = true;
			} else if (user_cmd == "ls -al" || user_cmd == "ls -la") {
				showAll = true;
				longFormat = true;
			} else {
				cout << "Unsupported ls option" << endl;
				continue;
			}

			DIR *dir = opendir(cwd);
			if (!dir) {
				cout << "Error opening directory " << cwd << endl;
				continue;
			}

			struct dirent *dir_entry;
			int i = 0;
			while ((dir_entry = readdir(dir)) != 0 && i < 256) {
				names[i++] = dir_entry->d_name;
			}
			closedir(dir);

			sort(names, i);

			if (!longFormat) {
				int p = 0;
				for (int j = 0; j < i; j++) {
					if (!showAll && names[j][0] == '.') continue;
					p += static_cast<int>(names[j].length()) + 2;
					if (p > 80) {
						cout << endl;
						p = static_cast<int>(names[j].length()) + 2;
					}
					cout << names[j] << "  ";
				}
				cout << endl;
			} else {
				for (int j = 0; j < i; j++) {
					if (!showAll && names[j][0] == '.') continue;

					string fullPath = string(cwd) + "/" + names[j];
					struct stat st;
					if (lstat(fullPath.c_str(), &st) != 0) {
						cout << "Cannot stat " << names[j] << endl;
						continue;
					}

					struct passwd *pw = getpwuid(st.st_uid);
					struct group *gr = getgrgid(st.st_gid);
					string owner = pw ? pw->pw_name : "unknown";
					string group = gr ? gr->gr_name : "unknown";

					cout << fileType(st.st_mode)
					     << formatPermissions(st.st_mode) << " "
					     << "inode=" << st.st_ino << " "
					     << "size=" << st.st_size << " "
					     << "owner=" << owner << ":" << group << " "
					     << "created(chg)=" << fomratTime(st.st_ctime) << " "
					     << "modified=" << fomratTime(st.st_mtime) << " "
					     << names[j] << endl;
				}
			}
		}
    	}
	return 0;
}
