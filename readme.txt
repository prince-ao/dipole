The Dipole Programming Language
---
Grammar:
<program>_S ::= <statement>* EOT ;
<statement> ::= <printStmt> | 
	<ifStmt> | 
	<declStmt> | 
	<assignStmt> ;
<printStmt> ::= 'print' <expression> 'new_line' ;
<ifStmt> ::= <ifHead> | <ifHead> 'else' <block> ;
<ifHead> ::= 'if' '(' <expression> ')' <block> ;
<block> ::= '{' <statement>* '}' ;
<declStmt> ::= 'let' 'identifier' ( '=' <expression> )? 'new_line' ;
<assignStmt> ::= 'identifier' '=' <expression> 'new_line' ;
<expression> ::= <equality> ;
<equality> ::= <comparison> (('!=' | '==') <comparison>)* ;
<comparison> ::= <term> (('>' | '>=' | '<' | '<=') <term>)* ;
<term> ::= <factor> (('+' | '-') <factor>)* ;
<factor> ::= <unary> (('*' | '/') <unary>)* ;
<unary> ::= ('-' | '!') <unary> | <primary> ;
<primary> ::= 'int' | 
	'true' | 
	'false' | 
	'none' | 
	'identifier' | 
	'string' |
	(' <expression> ')' ;
