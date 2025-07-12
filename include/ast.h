#ifndef AST_H
# define AST_H

typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SEMI,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_REDIR_IN,
    TOKEN_HEREDOC
} t_token_type;

typedef struct s_redir {
    t_token_type    type;
    char            *file;
    struct s_redir  *next;
} t_redir;

typedef struct s_cmd {
    char            **argv;
    t_redir         *redirs;
    struct s_cmd    *pipe_next;
    struct s_cmd    *next;
    t_token_type    next_type;
} t_cmd;

#endif