The Dipole Programming Language
---
Grammer:
<program>_S ::= ('print' <expression> )*
<expression> ::= <factor> (('+' | '-') <factor>)*
<factor> ::= <unary> (('*' | '/') <unary>)*
<unary> ::= '-' <unary> | int


