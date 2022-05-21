#include <stdio.h>
//exit(), system()
#include <stdlib.h>
//for get_time()
#include <time.h>
//for check_folder()
#include <dirent.h>
#include <errno.h>

#define MAX 100
#define max_order_a_day 100
#define max_length 100

char receipt_list[max_order_a_day][35];
int count = 0;

void check_folder();
void cn_read_menu(char name[][max_length], double cost[], int index[], int* n);
void open_file(FILE **f, char* file_name, char *mode);
void cn_input_transaction_code();
void cn_print_menu(char name[][max_length], double cost[], int number_of_dishes, int index[]);
void cn_order(char name[][max_length], double cost[MAX], int index[MAX], int number_of_dishes);
int string_to_number(char string[]);
int string_length(char string[]);
int string_compare(char string_1[], char string_2[]);
void string_concatenation(char des[], char string_1[], char string_2[]);
void string_copy(char des[], char src[]);
void rm_invalid(char src[]);
void remove_newline(char string[]);
int cn_check_order(int tmp[], char name[][max_length], double cost[], int n);
void wait(char name[][max_length], double cost[], int number_of_dishes, int index[], char transaction_code[]);
int quantity(int num);
void cn_print_receipt(int tmp[], char name[][max_length], double cost[], double total, double dis, int n);
char* get_time();
double discount();
void end_day();

int main() {
	char name[MAX][max_length], transaction_code[100] = "12345";
	double cost[MAX];
	int index[MAX];
	int number_of_dishes;
	
	check_folder();
	cn_read_menu(name, cost, index, &number_of_dishes);

//	cn_input_transaction_code(&transaction_code);
//	cn_print_menu(name, cost, number_of_dishes, index);
	wait(name, cost, number_of_dishes, index, transaction_code);
//	for (int i = 0; i < 5; i++) {
//		printf("%d", name[i]);
//	}

	return 0;
}

void check_folder() {
	DIR* dir = opendir("receipt");
	if (dir) {
	    closedir(dir);
	} else if (ENOENT == errno) {
	    printf("Missing \"receipt\" folder. Please create one.");
	    exit(1);
	} else {
	    printf("Error: opendir() failed...");
	    exit(1);
	}
}
void cn_read_menu(char name[][max_length], double cost[], int index[], int* i) {
	FILE* f;
	open_file(&f, "menu.txt", "r");
	char line[MAX];

	while (!feof(f)) {
		fgets(line, MAX, f);
		index[*i] = string_to_number(line);

		fgets(line, MAX, f);
		string_copy(name[*i], line);
		remove_newline(name[*i]);

		fgets(line, MAX, f);
		cost[*i] = string_to_number(line);

		fgets(line, MAX, f);

		(*i)++;
	}

	fclose(f);
}
void open_file(FILE **file, char* file_name, char *mode) {
    *file = fopen(file_name, mode);
    if (!*file) {
    	printf("Can not open file.\n");
		perror("Because");
//		printf("Because can't find menu.txt file");
		exit(1);
	}
}
void cn_input_transaction_code(char* code) {
	printf("Type your transaction code: ");
	scanf("%s", code);
	system("cls");
}
void cn_print_menu(char name[][max_length], double cost[], int number_of_dishes, int index[]) {
//	printf("==============MENU==============\n\n\n");
	printf("\t#     #    #######    #     #    #     #\n");
	printf("\t##   ##    #          ##    #    #     #\n");
	printf("\t# # # #    #          # #   #    #     #\n");
	printf("\t#  #  #    #####      #  #  #    #     #\n");
	printf("\t#     #    #          #   # #    #     #\n");
	printf("\t#     #    #          #    ##    #     #\n");
	printf("\t#     #    #######    #     #     #####\n\n\n");	
	printf("ID\tNAME:\t\t\t\t\tCOST:\n");
	for (int i = 0; i < number_of_dishes; i++) {
		printf("[%d]\t", index[i]);
		puts(name[i]);
		printf("\t\t\t\t\t\t%.0lf", cost[i]); printf("\n");
		printf("------------------------------------------------------------\n");
	}
}
void cn_order(char name[][max_length], double cost[MAX], int index[MAX], int number_of_dishes) {
	char input[50];
	int order[100];
	int tmp = 0, i = 0;
	cn_print_menu(name, cost, number_of_dishes, index);
	printf("\n\n\n[0 0]: confirm\t");
	printf("[0]: undo\n");
	printf("Choose your dishes (MAX = 5):\n");
	for (i = 0; ; i++) {
		if (i == 5) {
//			printf("You have reach maximum order. Please confirm or undo...\n");
//			fgets(input, sizeof(input), stdin); 
//			if (input[0] != '0') {
//				i--;
//				continue;
//			}
			string_copy(input, "0 0");
		} else {
			fgets(input, sizeof(input), stdin);
		}
		if (string_compare(input, "0 0")) {
			if (cn_check_order(order, name, cost, i)) {
				system("cls");
				break;
			} else {
				system("cls");
				cn_print_menu(name, cost, number_of_dishes, index);
				printf("\n\n\n[0 0]: confirm\t");
				printf("[0]: undo\n");
				i--;
				continue;
			}
		}
		if ((input[0] >= '0' && input[0] <= '9') || input[0] == '-') {
			tmp = string_to_number(input) - 1;
			if (tmp == -1) {
				i--;
				order[i] = 0;
				i--;
				continue;
			}
			if (tmp > number_of_dishes - 1 || tmp < -1) {
				printf("We don't have that dish\n");
				i--;
			} else {
				tmp = quantity(tmp);
				order[i] = tmp;
			}
		} else {
			remove_newline(input);
			for (int j = 0; j < number_of_dishes; j++) {
				if (string_compare(input, name[j])) {
					tmp = j;
					tmp = quantity(tmp);
					order[i] = tmp;
					break;
				}
				if (j == number_of_dishes - 1) {
					printf("We don't have that dish %d %d\n", i, j);
					i--;
				}
			}
		}
	}
}
int string_to_number(char string[]) {
	int result = 0, sign = 1, i = 0;
	remove_newline(string);
	if (string[0] == '-') {
		sign = -1;
		i++;
	}
	for ( ; string[i] != '\0'; i++) {
		result = result*10 + string[i] - '0'; // '0' == 48
	}
	return sign*result;
}
int string_length(char string[]) {
	size_t length = 0;
	while(string[length] != '\0'){
		length++;
	}
	return length;
}
int string_compare(char string_1[], char string_2[]) {
	remove_newline(string_1);
	remove_newline(string_2);
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
void string_copy(char des[], char src[]) {
	remove_newline(des);
	remove_newline(src);
	int i;
	for (i = 0; src[i] != '\0'; i++) {
		des[i] = src[i];
	}
	des[i] = '\0';
}
void rm_invalid(char src[]) {
	int i = 0;
	while (src[i]) {
		if (src[i] == ' ' || src[i] == ':') {
			src[i] = '-';
		}
		i++;
	}
}
void remove_newline(char string[]) {
	size_t ln;
	ln = string_length(string) - 1;
	if (string[ln] == '\n')
	string[ln] = '\0';
}
int cn_check_order(int tmp[], char name[][max_length], double cost[], int n) {
	int check;
	int qty, dish;
	double total = 0, dis = 0;
	system("cls");
	printf("Please confirm your order.\n\n");
	printf("NAME\t\t\tQTY.\t\t\tCOST\n\n");
	printf("------------------------------------------------------------\n");
	for (int i = 0; i < n; i++) {
		qty = tmp[i] % 10;
		dish = tmp[i] / 10;
		puts(name[dish]);
		printf("\t\t\t%d", qty);
		total = total + cost[dish]*qty;
		printf("\t\t\t%.0lf\n", cost[dish]*qty);
		printf("------------------------------------------------------------\n\n");
	}
	printf("TOTAL:\t\t\t\t\t\t%.0lf\n", total);
	dis = discount(total);
	printf("DISCOUNT:\t\t\t\t\t%.0lf\n", dis);
	printf("NET:\t\t\t\t\t\t%.0lf\n\n\n\n", total - dis);
	if (n == 5) {
		system("pause");
		check = 1;
	} else {
		do {
			printf("[1]: confirm\t");
			printf("[0]: back\n");
			scanf("%d", &check);
			fflush(stdin);
			system("cls");
		} while (check != 1 && check != 0);
	}
	if (check) cn_print_receipt(tmp, name, cost, total, dis, n);
	return check;
}
void string_concatenation(char des[], char string_1[], char string_2[]) {
	remove_newline(des);
	remove_newline(string_1);
	remove_newline(string_2);
    int i = 0, j = 0;
    while (string_1[i] != '\0') {
        des[j] = string_1[i];
        i++;
        j++;
    }
    i = 0;
    while (string_2[i] != '\0') {
        des[j] = string_2[i];
        i++;
        j++;
    }
    des[j] = '\0';
}
void wait(char name[][max_length], double cost[], int number_of_dishes, int index[], char transaction_code[]) {
	char input[100];
	while (1) {
		printf("[1]: order\t");
		printf("[transaction_code]: end\n");
		scanf("%s", &input);
		fflush(stdin);
		system("cls");
		if (string_compare(input, "1")) {
//			cn_print_menu(name, cost, number_of_dishes, index);
			cn_order(name, cost, index, number_of_dishes);
		} else if (string_compare(input, transaction_code)) {
			end_day();
		}
	}
}
int quantity(int num) {
	int qty = 0;
	while (qty == 0) {
		printf("Quantity (MAX < 10):");
		do {
			scanf("%d", &qty);
		} while (qty >= 10);
		fflush(stdin);
		num = num*10 + qty;
	}
	return num;
}
void cn_print_receipt(int tmp[], char name[][max_length], double cost[], double total, double dis, int n) {
	FILE* f;
	int qty, dish;
	char path[1000];
	string_concatenation(path, "receipt", "\\\\");
	string_concatenation(path, path, get_time());
	string_concatenation(path, path, ".txt");
	rm_invalid(path);
	
	string_copy(receipt_list[count], path); count++;
	
	open_file(&f, path, "w");
	fprintf(f, "NAME\t\t\tQTY.\t\t\tCOST\n\n");
	for (int i = 0; i < n; i++) {
		qty = tmp[i] % 10;
		dish = tmp[i] / 10;
		fprintf(f, "%s\n", name[dish]);
		fprintf(f, "\t\t\t%d", qty);
		fprintf(f, "\t\t\t%.0lf\n", cost[dish]*qty);
		fprintf(f, "--------------------------------------------------------------------------------------------\n\n");
	}
	fprintf(f, "TOTAL:\t\t\t\t\t\t%.0lf\n", total);
	fprintf(f, "DISCOUNT:\t\t\t\t\t%.0lf\n", dis);
	fprintf(f, "NET:\t\t\t\t\t\t%.0lf\n\n\n\n", total - dis);
	fclose(f);
}
char* get_time() {
	time_t now = time(NULL);
	struct tm* ptm;
	return ctime(&now);
}
double discount(double n) {
	if (n >= 2000000) n = n*25/100;
	else n = 0;
	return n;
}
void end_day() {
	for (int i = 0; i < count; i++) {
		puts(receipt_list[i]);
	}
	system("pause");
	exit(0);
}
