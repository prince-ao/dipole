The Dipole Programming Language
---
Grammar:
<program>_S ::= <statement>* EOT ;
<statement> ::= <printStmt> ;
<printStmt> ::= 'print' <expression> 'new_line' ;
<expression> ::= <equality> ;
<equality> ::= <comparison> (('!=' | '==') <comparison>)* ;
<comparison> ::= <term> (('>' | '>=' | '<' | '<=') <term>)* ;
<term> ::= <factor> (('+' | '-') <factor>)* ;
<factor> ::= <unary> (('*' | '/') <unary>)* ;
<unary> ::= ('-' | '!') <unary> | <primary> ;
<primary> ::= 'int' | 'true' | 'false' | 'none' | '(' <expression> ')' ;
