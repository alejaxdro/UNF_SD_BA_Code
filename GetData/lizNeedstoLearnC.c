#include <stdio.h>
#include<string.h>

void print_string( char *Name);

main(){
	 char Name[100];
	// float Test2;
	// float Test3;
	// char average;
	printf("Enter your name:\n");
	scanf("%s", Name);
	

}// end

void print_string( char *Name) {
		printf("your name is: %s ", &Name);
	}