<program> ::= <routine>*
<routine> ::= <label> ":" <action>*
<action> ::= (">" <label> | <variable> "=" <value>) ";"
<value> ::= <number> | <variable> | "(" <value> "+" <value> ")"
<variable> ::= [a-z]+
<label> ::= [A-Z]+
<number> ::= [0-9]+