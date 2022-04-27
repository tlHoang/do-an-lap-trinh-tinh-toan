#include <stdio.h>
#include <stdlib.h> //atoi(), exit()
#include <string.h> //strcpy(), strlen()

#define MAX 100

void read_menu(char name[][MAX], double cost[], int index[], int* n);
void open_file(FILE **f, char* file_name, char *mode);
void input_transaction_code();
void print_menu(char name[][MAX], double* cost, int number_of_dishes);

int main() {
	char name[MAX][MAX], transaction_code[100];
	double cost[MAX];
	int index[MAX];
	int number_of_dishes;

	read_menu(name, cost, index, &number_of_dishes);

	input_transaction_code(&transaction_code);

	print_menu(name, cost, number_of_dishes);

	return 0;
}

void read_menu(char name[][MAX], double cost[], int index[], int* i) {
	FILE* f;
	open_file(&f, "menu.txt", "r");
	char line[MAX];
	size_t ln;

	while (!feof(f)) {
		fgets(line, MAX, f);
		index[*i] = atoi(line);

		fgets(line, MAX, f);
		strcpy(name[*i], line);
		//remove \n after name
		ln = strlen(name[*i]) - 1;
		if (name[*i][ln] == '\n')
 		name[*i][ln] = '\0';

		fgets(line, MAX, f);
		cost[*i] = atof(line);

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
void print_menu(char name[][MAX], double* cost, int number_of_dishes) {
	printf("STT  NAME:\t\tCOST:\n");
	for (int i = 0; i < number_of_dishes; i++) {
		printf("[%d]  ", i+1); puts(name[i]); printf("\t\t\t%.0lf", cost[i]); printf("\n");
		printf("--------------------------------\n");
	}
}
