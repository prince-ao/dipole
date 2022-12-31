#include "../includes/includes.h"
#include "../includes/lexer.h"
#include "../includes/parser.h"
#include "../includes/interpreter.h"

#define BUF_SIZE 4096

int main(int argc, char *argv[]){
	if(argc != 2){
		fputs("Usage: dipole <pathname>\n", stderr);
		exit(1);
	}

	Lexer lexer;
	Parser parser(&lexer);
	Interpreter interpreter;
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
	fclose(prgm);

	//lexer.print_tokens();
	//puts("--end of tokens---");

	AstNode *p = parser.parse_program();
	//parser.print_ast(p, 0);
	//puts("---end of AST---");

	interpreter.interpret(p);

	exit(0);
}
