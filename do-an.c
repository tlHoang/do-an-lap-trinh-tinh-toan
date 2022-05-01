#include <stdio.h>
#include <stdlib.h> //exit()
#include <string.h> //strcpy()
//#include <ctype.h>

#define MAX 100

void read_menu(char name[][MAX], double cost[], int index[], int* n);
void open_file(FILE **f, char* file_name, char *mode);
void input_transaction_code();
void print_menu(char name[][MAX], double cost[], int number_of_dishes, int index[]);
void order(char name[][MAX], double cost[MAX], int index[MAX], int number_of_dishes);
int simplify_string_to_number(char string[]);
int string_length(char string[]);
int compare_string(char string_1[], char string_2[]);

int main() {
	char name[MAX][MAX], transaction_code[100] = "12345";
	double cost[MAX];
	int index[MAX];
	int number_of_dishes;

	read_menu(name, cost, index, &number_of_dishes);

//	input_transaction_code(&transaction_code);

	print_menu(name, cost, number_of_dishes, index);
	
	order(name, cost, index, number_of_dishes);
	
	return 0;
}

void read_menu(char name[][MAX], double cost[], int index[], int* i) {
	FILE* f;
	open_file(&f, "menu.txt", "r");
	char line[MAX];
	size_t ln;

	printf("==============MENU==============\n\n\n");
	while (!feof(f)) {
		fgets(line, MAX, f);
		index[*i] = simplify_string_to_number(line);

		fgets(line, MAX, f);
		strcpy(name[*i], line);
		//remove \n after name
		ln = string_length(name[*i]) - 1;
		if (name[*i][ln] == '\n')
 		name[*i][ln] = '\0';

		fgets(line, MAX, f);
		cost[*i] = simplify_string_to_number(line);

		fgets(line, MAX, f);

		(*i)++;
	}

	fclose(f);
}
void open_file(FILE **file, char* file_name, char *mode) {
    *file = fopen(file_name, mode);
    if (!*file) {
    	printf("Can not open file.\n");
//		perror("Because");
		printf("Because can't find menu.txt file");
		exit(1);
	}
}
void input_transaction_code(char* code) {
	printf("Type your transaction code: ");
	scanf("%s", code);
	system("cls");
}
void print_menu(char name[][MAX], double cost[], int number_of_dishes, int index[]) {
	printf("ID   NAME:\t\tCOST:\n");
	for (int i = 0; i < number_of_dishes; i++) {
		printf("[%d]  ", index[i]); puts(name[i]); printf("\t\t\t%.0lf", cost[i]); printf("\n");
		printf("--------------------------------\n");
	}
}
void order(char name[][MAX], double cost[MAX], int index[MAX], int number_of_dishes) {
	char input[50];
	int order[100];
	int tmp = 0, n = 0;
	for (int i = 0; ; i++) {
		printf("Choose your dishes:\n");
		fgets(input, sizeof(input), stdin);
		if (input[0] >= '0' && input[0] <= '9') {
			tmp = simplify_string_to_number(input);
			if (tmp == 0) break;
			order[i] = tmp;
			n++;
		} else {
			for (int j = 0; j < number_of_dishes; j++) {
				
			}
		}
	}
//	for (int i = 0; i < 5; i++) {
//		printf("%d ", order[i]);
//	}
}
int simplify_string_to_number(char string[]) {
	int result;
	size_t ln;
	ln = string_length(string) - 1;
	if (string[ln] == '\n')
	string[ln] = '\0';
	for (int i = 0; ; i++) {
		if (string[i] != '\0') {
			result = 10*result + string[i] - '0'; //'0' == 48
		} else {
			break;
		}
	}
	return result;
}
int string_length(char string[]) {
	size_t length = 0;
	while(string[length] != '\0'){
		length++;
	}
	return length;
}
int compare_string(char string_1[], char string_2[]) {
	int i;
	for (i = 0; string_1[i] && string_2[i]; i++) {
		if (string_1[i] == string_2[i] || (string_1[i] ^ 32) == string_2[i]) { //Vi trong ma ascii chu hoa va thuong cach nhau 32 don vi
			continue;														   //32(10) == 100000(2) => chi khac nhau bit thu 6
		} else {															   //Dung phep xor cho bit thu 6 neu giong nhau thi la cung 1 chu
			break;
		}
	}
	if (string_1[i] == string_2[i]) {
		return 1;
	} else {
		return 0;
	}
}
