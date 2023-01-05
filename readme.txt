The Dipole Programming Language
---
Grammar:
<program>_S ::= ('print' <expression> )* ;
<expression> ::= <equality> ;
<equality> ::= <comparison> (('!=' | '==') <comparison>)* ;
<comparison> ::= <term> (('>' | '>=' | '<' | '<=') <term>)* ;
<term> ::= <factor> (('+' | '-') <factor>)* ;
<factor> ::= <unary> (('*' | '/') <unary>)* ;
<unary> ::= ('-' | '!') <unary> | <primary> ;
<primary> ::= 'int' | 'true' | 'false' | 'none' | '(' <expression> ')' ;
