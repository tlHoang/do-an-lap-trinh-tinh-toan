void remove_newline(char string[]) {
	size_t ln;
	ln = string_length(string) - 1;
	if (string[ln] == '\n')
	string[ln] = '\0';
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
		if (string_1[i] == string_2[i] || (string_1[i] ^ 32) == string_2[i]) continue; //Vi trong ma ascii chu hoa va thuong cach nhau 32 don vi	
		else break;                                                                    //32(10) == 100000(2) => chi khac nhau bit thu 6              
	}                                                                                  //Dung phep xor cho bit thu 6 neu giong nhau thi la cung 1 chu
	if (string_1[i] == string_2[i]) return 1;
	else return 0;
}
void string_copy(char des[], char src[]) {
	remove_newline(des);
	remove_newline(src);
	int i;
	for (i = 0; src[i] != '\0'; i++) des[i] = src[i];
	des[i] = '\0';
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
