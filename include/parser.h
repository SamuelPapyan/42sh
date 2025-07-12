#ifndef PARSER_H
# define PARSER_H

#include "ast.h"

t_cmd   *parser_tokens(char **tokens);
void    free_cmd(t_cmd *cmd);

#endif