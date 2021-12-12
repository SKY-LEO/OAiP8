//#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <conio.h>
#include <iostream>
#include <ctime>
using namespace std;
const int NUMBER_OF_SYMBOLS = 21;
const int MINIMUM_GOOD_MARK = 4;
const int MINIMUM_MARK = 0;
const int MAXIMUM_MARK = 10;

struct initials
{
	char surname[NUMBER_OF_SYMBOLS];
	char name[NUMBER_OF_SYMBOLS];
	char patronymic[NUMBER_OF_SYMBOLS];
};

struct marks
{
	int physics;
	int math;
	int informatics;
	int chemistry;
};

struct date_of_birth
{
	int day;
	int month;
	int year;
};

struct student
{
	initials FIO;
	date_of_birth births;
	char group_number[NUMBER_OF_SYMBOLS];
	marks subjects;
	double average_score;
};

int SIZE = sizeof(student);
int createFile(FILE* Data);
int viewFile(FILE* Data, FILE* Info, student& note);
int addToFile(FILE* Data, student& note);
int sortInformation(FILE* Data, FILE* Info, FILE* Info_backup, student* arr_of_students);
void Out(FILE* Info, student z);
void Backup(FILE* Info, student z);
void checkInputString(char* my_string, int type_of_data);// 0 - для ФИО, 1 - для группы
bool checkDateOfBirth(int& day, int& month, int& year);
bool checkMarks(int& mark);
int correctInputInt();

int main()
{
	FILE* Data = 0, * Info = 0, * Info_backup = 0;
	student note, * arr_of_students = 0;
	while (true)
	{
		int code = 0;
		do
		{
			printf("\n Create - 1\n View - 2\n Add - 3\n Individual task - 4\n EXIT - 0\n");
			scanf_s("%d", &code);
		} while (code < 0 || code > 4);
		switch (code)
		{
		case 1: createFile(Data);
			break;
		case 2: viewFile(Data, Info, note);
			break;
		case 3: addToFile(Data, note);
			break;
		case 4: sortInformation(Data, Info, Info_backup, arr_of_students);
			break;
		case 0: system("pause");
			return 0;
		}
	}
}

int createFile(FILE* Data)
{
	if (fopen_s(&Data, "data.dat", "rb") != NULL) {
		printf("\nERROR open file!");
		if (fopen_s(&Data, "data.dat", "wb") != NULL)
		{
			return -1;
		}
	}
	int length = _filelength(_fileno(Data));
	fclose(Data);
	if (length)
	{
		char code;
		printf("\nThere is some information in the file. Do you really want to create new file?(Y/N)\n");
		code = _getch();
		if (code == 'Y' || code == 'y')
		{
			if ((fopen_s(&Data, "data.dat", "wb")) != NULL)
			{
				printf("\nCreate ERROR!");
			}
			else
			{
				fclose(Data);
				printf("\nCreate new file!");
			}
		}
	}
	else
	{
		if ((fopen_s(&Data, "data.dat", "wb")) != NULL)
		{
			printf("\nCreate ERROR!");
		}
		else
		{
			fclose(Data);
			printf("\nCreate new file!");
		}
	}
	return 0;
}

int viewFile(FILE* Data, FILE* Info, student& note)
{
	if ((fopen_s(&Data, "data.dat", "rb")) != NULL)
	{
		printf("\nOpen ERROR!\n");
		return -1;
	}
	else
	{
		char code;
		printf("\nDo you want to save data to default file?(Y/N)\n");
		code = _getch();
		if (code == 'Y' || code == 'y')
		{
			if (fopen_s(&Info, "results.txt", "w") != NULL)
			{
				printf("Open ERROR!");
				return -1;
			}
		}
		else
		{
			char file_name[NUMBER_OF_SYMBOLS];
			printf("To which file do you want save info?\n");
			checkInputString(file_name, 2);
			if (fopen_s(&Info, file_name, "w") != NULL)
			{
				printf("Open ERROR!");
				return -1;
			}
		}
		printf("\n----------------INFORMATION----------------\n");
		printf("\n %12s %20s %14s %14s %10s %16s %10s %15s", "F.I.O.", "Date of birth", "Group", "Physics", "Math", "Informatics", "Chemistry", "Average score");
		fprintf(Info, "\n----------------INFORMATION----------------\n");
		fprintf(Info, "\n %12s %20s %14s %14s %10s %16s %10s %15s", "F.I.O.", "Date of birth", "Group", "Physics", "Math", "Informatics", "Chemistry", "Average score");
		while (1) {
			if (fread(&note, SIZE, 1, Data) == 0) break;
			Out(Info, note);
		}
		fclose(Info);
		fclose(Data);
	}
	return 0;
}

int addToFile(FILE* Data, student& note)
{
	int n = 0, i = 0;
	do
	{
		printf("How many people do you want to add?: ");
		n = correctInputInt();
	} while (n < 1);
	do
	{
		if (fopen_s(&Data, "data.dat", "ab") != NULL)
		{
			printf("Open ERROR!");
			return -1;
		}
		printf("\nSurname:\n");
		checkInputString(note.FIO.surname, 0);
		printf("\nName:\n");
		checkInputString(note.FIO.name, 0);
		printf("\nPatronymic:\n");
		checkInputString(note.FIO.patronymic, 0);
		bool flag = true;
		do
		{
			printf("\nDate of birth(DD MM YEAR): ");
			scanf_s("%d %d %d", &note.births.day, &note.births.month, &note.births.year);
			flag = checkDateOfBirth(note.births.day, note.births.month, note.births.year);
		} while (!flag);
		printf("Group number:\n");
		checkInputString(note.group_number, 1);
		printf("\n");
		do
		{
			printf("Physics: ");
			note.subjects.physics = correctInputInt();
			flag = checkMarks(note.subjects.physics);
		} while (!flag);
		do
		{
			printf("Maths: ");
			note.subjects.math = correctInputInt();
			flag = checkMarks(note.subjects.math);
		} while (!flag);
		do
		{
			printf("Informatics: ");
			note.subjects.informatics = correctInputInt();
			flag = checkMarks(note.subjects.informatics);
		} while (!flag);
		do
		{
			printf("Chemistry: ");
			note.subjects.chemistry = correctInputInt();
			flag = checkMarks(note.subjects.chemistry);
		} while (!flag);
		note.average_score = ((double)note.subjects.physics + note.subjects.math + note.subjects.informatics + note.subjects.chemistry) / 4.;
		printf("Average score: %.3f", note.average_score);
		fwrite(&note, SIZE, 1, Data);
		fclose(Data);
		i++;
	} while (i < n);
	return 0;
}

int sortInformation(FILE* Data, FILE* Info, FILE* Info_backup, student* arr_of_students)
{
	int length, quantity;
	if (fopen_s(&Data, "data.dat", "rb") != NULL)
	{
		printf("Open ERROR!");
		return -1;
	}
	length = _filelength(_fileno(Data));
	quantity = length / SIZE;
	arr_of_students = new student[quantity];
	for (int i = 0; i < quantity; i++)
	{
		fread((arr_of_students + i), SIZE, 1, Data);
	}
	fclose(Data);
	printf("Do you want to save initial data?(Y/N)");
	char code;
	code = _getch();
	if (code == 'Y' || code == 'y')
	{
		if (fopen_s(&Info_backup, "intitialData.txt", "wt") != NULL)
		{
			printf("Open ERROR!");
			return -1;
		}
		fprintf(Info_backup, "\n----------------INFORMATION----------------\n");
		for (int i = 0; i < quantity; i++)
		{
			Backup(Info_backup, arr_of_students[i]);
		}
		printf("\nSuccessfully saved!");
		fclose(Info_backup);
	}
	printf("\nDo you want to save data to default file?(Y/N)");
	code = _getch();
	if (code == 'Y' || code == 'y')
	{
		if (fopen_s(&Info, "results.txt", "w") != NULL)
		{
			printf("Open ERROR!");
			return -1;
		}
	}
	else
	{
		char file_name[NUMBER_OF_SYMBOLS];
		printf("\nTo which file do you want save info?\n");
		checkInputString(file_name, 2);
		if (fopen_s(&Info, file_name, "w") != NULL)
		{
			printf("Open ERROR!");
			return -1;
		}
	}
	printf("\n----------------SORT----------------\n");
	printf("\n %12s %20s %14s %14s %10s %16s %10s %15s", "F.I.O.", "Date of birth", "Group", "Physics", "Math", "Informatics", "Chemistry", "Average score");
	fprintf(Info, "\n----------------SORT----------------\n");
	fprintf(Info, "\n %12s %20s %14s %14s %10s %16s %10s %15s", "F.I.O.", "Date of birth", "Group", "Physics", "Math", "Informatics", "Chemistry", "Average score");
	for (int i = 0; i < quantity; i++)
	{
		if (arr_of_students[i].subjects.math >= MINIMUM_GOOD_MARK && arr_of_students[i].subjects.informatics >= MINIMUM_GOOD_MARK)
		{
			Out(Info, arr_of_students[i]);
		}
	}
	fclose(Info);
	delete[]arr_of_students;
	return 0;
}

void Out(FILE* Info, student z)
{
	printf("\n| %0s %2s %2s | %8.2d.%0.2d.%0.3d | %14.10s | %10.d | %10.d | %10.d | %10.d | %10.3f |", z.FIO.surname, z.FIO.name, z.FIO.patronymic, z.births.day, z.births.month, z.births.year, z.group_number, z.subjects.physics, z.subjects.math, z.subjects.informatics, z.subjects.chemistry, z.average_score);
	fprintf(Info, "\n| %0s %2s %2s | %8.2d.%0.2d.%0.3d | %14.10s | %10.d | %10.d | %10.d | %10.d | %10.3f |", z.FIO.surname, z.FIO.name, z.FIO.patronymic, z.births.day, z.births.month, z.births.year, z.group_number, z.subjects.physics, z.subjects.math, z.subjects.informatics, z.subjects.chemistry, z.average_score);
}

void Backup(FILE* Info, student z)
{
	fprintf(Info, "\n| %0s %2s %2s | %8.2d.%0.2d.%0.3d | %14.10s | %10.d | %10.d | %10.d | %10.d | %10.3f |", z.FIO.surname, z.FIO.name, z.FIO.patronymic, z.births.day, z.births.month, z.births.year, z.group_number, z.subjects.physics, z.subjects.math, z.subjects.informatics, z.subjects.chemistry, z.average_score);
}

int correctInputInt()
{
	int a;
	while (true)
	{
		cin >> a;
		if (cin.get() == '\n') {
			break;
		}
		else
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Error, please write INT numbers!\n" << "Try again!" << endl;
		}
	}
	return a;
}

void checkInputString(char* my_string, int type_of_data)
{
	int i = 0;
	while (i < NUMBER_OF_SYMBOLS - 1)//последний элемент для терминатора
	{
		my_string[i] = _getch();
		if (my_string[i] == 13) break;//enter
		if (my_string[i] == 8) {//backspace
			cout << "\b \b";
			i--;
			continue;
		}
		if (type_of_data == 0)
		{
			if (!((my_string[i] >= 'A' && my_string[i] <= 'Z') || (my_string[i] >= 'a' && my_string[i] <= 'z') || my_string[i] == '.'))
			{
				continue;
			}
		}
		else if (type_of_data == 1)
		{
			if (!(my_string[i] >= '0' && my_string[i] <= '9'))
			{
				continue;
			}
		}
		else if (type_of_data == 2)
		{
			if (!((my_string[i] >= 'A' && my_string[i] <= 'Z') || (my_string[i] >= 'a' && my_string[i] <= 'z') || my_string[i] == '_' || my_string[i] == '-' || my_string[i] == '.' || (my_string[i] >= '0' && my_string[i] <= '9')))
			{
				continue;
			}
		}
		cout << my_string[i];
		i++;
	}
	my_string[i] = '\0';
}

bool checkDateOfBirth(int& day, int& month, int& year)
{
	time_t now = time(0);
	struct tm ltm;
	localtime_s(&ltm, &now);
	if (day > 31 || day < 01)
	{
		printf("Wrong day!");
		return false;
	}
	else if (month > 12 || month < 01)
	{
		printf("Wrong month!");
		return false;
	}
	else if (year > 1900 + ltm.tm_year || year < (1900 + ltm.tm_year) - 120)
	{
		printf("Wrong year!");
		return false;
	}
	else if (year == 1900 + ltm.tm_year && month - 1 > ltm.tm_mon)//январь - 0
	{
		printf("This month hasn't come!");
		return false;
	}
	else if (year == 1900 + ltm.tm_year && month - 1 == ltm.tm_mon && day > ltm.tm_mday)
	{
		printf("This day hasn't come!");
		return false;
	}
	else
	{
		return true;
	}
}

bool checkMarks(int& mark)
{
	if (mark > MAXIMUM_MARK || mark < MINIMUM_MARK)
	{
		printf("Wrong mark!\n");
		return false;
	}
	else
	{
		return true;
	}
}