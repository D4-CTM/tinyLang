%%
[\s\t\n]                                    // SKIP
"("                                         // OPEN_PARENTHESIS
")"                                         // CLOSE_PARENTHESIS
"{"                                         // OpenBracket
"}"                                         // CloseBracket
";"                                         // SEMICOLON
"="                                         // EQUAL
[a-zA-Z_][a-zA-Z0-9_]*                      // IDENTIFIER
[0-9]+                                      // DECIMAL_INTEGER_CONSTANT
[0-9]+\.[0-9]+                              // DECIMAL_FLOAT_CONSTANT
\"[^\n\"]\"                                 // StringLiteral
"//".*                                      // LineComment
"/*"(\n|.)*?"*/"                            // StartBlockComment /* */ test */
\"\"\"(([^\"]|\"[^\"]|\"\"[^\"]))*\"\"\"    // MultiLineStringLiteral
0[xX][0-9A-Fa-f]+                           // HEX_CONSTANT
0[bB][01]+                                  // BINARY_CONSTANT
0[oO][0-7]+                                 // OCTAL_CONSTANT
%%
