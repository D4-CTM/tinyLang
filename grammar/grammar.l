%%
[\s\t\n]                // SKIP
"("                     // OpenParenthesis
")"                     // CloseParenthesis
"{"                     // OpenBracket
"}"                     // CloseBracket
";"                     // Semicolon
[a-zA-Z_][a-zA-Z0-9_]*  // Identifier
[0-9]+                  // DecimalIntegerConstant
[0-9]*\.[0-9]+          // DecimalFloatConstant
%%
