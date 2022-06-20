#include <stdio.h>
//exit(), system()
#include <stdlib.h>
//for get_time()
#include <time.h>

#define MAX 100
#define max_order_a_day 100
#define max_length 100

char receipt_list[max_order_a_day][100];
int count = 0;

void open_file(FILE **f, char* file_name, char *mode);
void cn_read_menu(char name[][max_length], double cost[], int index[], int* n);
void cn_input_transaction_code();
void cn_wait(char name[][max_length], double cost[], int number_of_dishes, int index[], char transaction_code[]);
void cn_order(char name[][max_length], double cost[MAX], int index[MAX], int number_of_dishes);
int cn_check_order(int tmp[], char name[][max_length], double cost[], int n);
void cn_print_receipt(int tmp[], char name[][max_length], double cost[], double total, double dis, int n);
void end_day();
void cn_print_menu(char name[][max_length], double cost[], int number_of_dishes, int index[]);
//In Chuoi.h
int string_to_number(char string[]);
int string_length(char string[]);
int string_compare(char string_1[], char string_2[]);
void string_concatenation(char des[], char string_1[], char string_2[]);
void string_copy(char des[], char src[]);
int is_number(char string[]);
void remove_newline(char string[]);
//
int quantity(int num);
double discount(double n);
char* get_time();
void remove_invalid(char src[]);
int check_integer(double n);
void set_color(int code);

#include "Chuoi.h"
#include "Color.h"
#include "Folder.h"

int main() {
	SetConsoleTitle("PBL_1");
	
	char name[MAX][max_length], transaction_code[100];
	double cost[MAX];
	int index[MAX];
	int number_of_dishes;
	
	check_folder();
	cn_read_menu(name, cost, index, &number_of_dishes);

	cn_input_transaction_code(&transaction_code);
	cn_wait(name, cost, number_of_dishes, index, transaction_code);

	return 0;
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
//		perror("Because");
		printf("Because can't find menu.txt file");
		exit(1);
	}
}
void cn_input_transaction_code(char* code) {
	printf("Type your transaction code: ");
	scanf("%s", code);
	system("cls");
}
void cn_print_menu(char name[][max_length], double cost[], int number_of_dishes, int index[]) {
	set_color(red);
	printf("\t#     #    #######    #     #    #     #\n");
	printf("\t##   ##    #          ##    #    #     #\n");
	printf("\t# # # #    #          # #   #    #     #\n");
	printf("\t#  #  #    #####      #  #  #    #     #\n");
	printf("\t#     #    #          #   # #    #     #\n");
	printf("\t#     #    #          #    ##    #     #\n");
	printf("\t#     #    #######    #     #     #####\n\n\n");	
	set_color(lightBlue);
	printf("ID\tNAME:\t\t\t\t\tCOST:\n");
	set_color(white);
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
	int tmp = 0, i = 0, flag;
	cn_print_menu(name, cost, number_of_dishes, index);
	set_color(aqua);
	printf("\n\n\n[0 0]: confirm\t");
	printf("[0]: undo\n");
	set_color(yellow);
	printf("Choose your dishes (MAX = 5):\n");
	set_color(white);
	for (i = 0; ; i++) {
		printf("\nDish number %d: ", i + 1);
		if (i == 5) string_copy(input, "0 0");
		else fgets(input, sizeof(input), stdin);
		
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
		
		if (is_number(input)) {
			tmp = string_to_number(input) - 1;
			if (tmp == -1) {
				i--;
				order[i] = 0;
				i--;
			} else if (tmp > number_of_dishes - 1 || tmp < -1) {
				printf("We don't have that dish\n");
				i--;
			} else {
				tmp = quantity(tmp);
				order[i] = tmp;
			}
		} else {
			remove_newline(input);
			flag = 1;
			for (int j = 0; j < number_of_dishes; j++) {
				if (string_compare(input, name[j])) {
					tmp = j;
					tmp = quantity(tmp);
					order[i] = tmp;
					flag = 0;
					break;
				}
			}
			if (flag) {
				printf("We don't have that dish\n");
				i--;
			}
		}
	}
}
void remove_invalid(char src[]) {
	int i = 0;
	while (src[i]) {
		if (src[i] == ' ' || src[i] == ':') src[i] = '-';
		i++;
	}
}
int cn_check_order(int tmp[], char name[][max_length], double cost[], int n) {
	int check;
	int qty, dish;
	double total = 0, dis = 0;
	system("cls");
	set_color(yellow);
	printf("Please confirm your order.\n\n");
	set_color(lightBlue);
	printf("NAME\t\t\tQTY.\t\t\tCOST\n\n");
	set_color(white);
	printf("------------------------------------------------------------\n");
	for (int i = 0; i < n; i++) {
		qty = tmp[i] % 100;
		dish = tmp[i] / 100;
		puts(name[dish]);
		printf("\t\t\t%d", qty);
		total = total + cost[dish]*qty;
		printf("\t\t\t%.0lf\n", cost[dish]*qty);
		printf("------------------------------------------------------------\n\n");
	}
	printf("TOTAL:\t\t\t\t\t\t%.0lf\n", total);
	dis = discount(total);
	printf("DISCOUNT:\t\t\t\t\t%.0lf\n", dis);
	set_color(red);
	printf("NET:\t\t\t\t\t\t%.0lf\n\n\n\n", total - dis);
	set_color(white);
	if (n == 5) {
		system("pause");
		check = 1;
	} else {
		do {
			printf("[1]: confirm\t");
			printf("[0]: back\n");
			printf("\nINPUT: ");
			scanf("%d", &check);
			fflush(stdin);
			system("cls");
		} while (check != 1 && check != 0);
	}
	if (check) cn_print_receipt(tmp, name, cost, total, dis, n);
	return check;
}
void cn_wait(char name[][max_length], double cost[], int number_of_dishes, int index[], char transaction_code[]) {
	char input[100];
	while (1) {
		set_color(lightBlue);
		printf("[1]: order\t");
		printf("[transaction_code]: end\n");
		set_color(white);
		printf("\nINPUT: ");
		scanf("%s", &input);
		fflush(stdin);
		system("cls");
		if (string_compare(input, "1")) cn_order(name, cost, index, number_of_dishes);
		else if (string_compare(input, transaction_code)) end_day();
	}
}
int quantity(int num) {
	double qty = 0;

	printf("Quantity (1 <= Q <= 99): ");
	scanf("%lf", &qty);
	fflush(stdin);

	while (!check_integer(qty) || (qty > 99 || qty < 1)) {
		if (!check_integer(qty)) {
			printf("Integer only. Try again: ");
			scanf("%lf", &qty);
			fflush(stdin);
		} else {
			printf("Out of range. Try again: ");
			scanf("%lf", &qty);
			fflush(stdin);
		}
	}
	
	num = num * 100 + qty;
	return num;
}
void cn_print_receipt(int tmp[], char name[][max_length], double cost[], double total, double dis, int n) {
	FILE* f;
	int qty, dish;
	char path[100], time[100];
	
	string_copy(time, get_time());
	remove_invalid(time);
	string_copy(receipt_list[count], time); count++; //de in ra cac
	string_concatenation(path, "receipt\\\\", time);
	string_concatenation(path, path, ".txt");
	
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
	fprintf(f, "SUBTOTAL:\t\t\t\t\t\t%.0lf\n", total);
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
	FILE* f;
	open_file(&f, "report//bao-cao.txt", "a");
	
	set_color(yellow);
	printf("Today order:\n");
	set_color(white);
	for (int i = 0; i < count; i++) puts(receipt_list[i]);

	for (int i = 0; i < count; i++) {
		fputs(receipt_list[i], f);
		fputs("\n", f);
	}

	fclose(f);
	exit(0);
}
int check_integer(double n) {
	int m = (int)n;
	if (m == n) return 1;
	else return 0;
}
