#include "../includes/includes.h"

#define BUF_SIZE 4096

int main(int argc, char *argv[]){
	if(argc != 2){
		fputs("Usage: dipole <pathname>\n", stderr);
		exit(1);
	}

	Lexer lexer;
	char buf[BUF_SIZE];
	FILE *prgm;

	errno = 0;
	if((prgm = fopen(argv[1], "r")) == NULL){
		char temp_buf[strlen(argv[1]) + 21];
		sprintf(temp_buf, "error while opening %s", argv[1]);
		perror(temp_buf);
		exit(1);
	}

	while((fgets(buf, BUF_SIZE, prgm)) != NULL){
		lexer.lex(buf);
	}
	lexer.done();
	lexer.print_tokens();

	fclose(prgm);

	exit(0);
}
