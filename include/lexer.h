#ifndef LEXER_H
# define LEXER_H

char    **lexer_tokenize(const char *input);
void    free_tokens(char **tokens);

#endif