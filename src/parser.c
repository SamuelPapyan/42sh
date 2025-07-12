#include "42sh.h"

static t_token_type get_op_type(const char *s) {
    if (!strcmp(s, "|")) return TOKEN_PIPE;
    if (!strcmp(s, "&&")) return TOKEN_AND;
    if (!strcmp(s, "||")) return TOKEN_OR;
    if (!strcmp(s, ";")) return TOKEN_SEMI;
    if (!strcmp(s, ">")) return TOKEN_REDIR_OUT;
    if (!strcmp(s, ">>")) return TOKEN_REDIR_APPEND;
    if (!strcmp(s, "<")) return TOKEN_REDIR_IN;
    if (!strcmp(s, "<<")) return TOKEN_HEREDOC;
    return TOKEN_WORD;
}

static void add_arg(char ***args, int *argc, const char *val) {
    *args = realloc(*args, sizeof(char *) * (*argc + 2));
    (*args)[(*argc)++] = strdup(val);
    (*args)[*argc] = NULL;
}

static void add_redir(t_redir **lst, t_token_type type, const char *file) {
    t_redir *r = malloc(sizeof(t_redir));
    r->type = type;
    r->file = strdup(file);
    r->next = NULL;

    if (!*lst)
        *lst = r;
    else {
        t_redir *cur = *lst;
        while (cur->next) cur = cur->next;
        cur->next = r;
    }
}

t_cmd   *parse_tokens(char **tokens) {
    t_cmd *head = NULL, *curr = NULL;
    char **argv = NULL;
    int argc = 0;
    t_redir *redir = NULL;

    for (int i = 0; tokens[i]; i++) {
        t_token_type type = get_op_type(tokens[i]);
        
        if (type == TOKEN_WORD) {
            add_arg(&argv, &argc, tokens[i]);
        }
        else if (type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND
            || type== TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
        {
            if (!tokens[i + 1]) break;
            add_redir(&redir, type, tokens[++i]);
        }
        else {
            t_cmd *new = malloc(sizeof(t_cmd));
            new->argv = argv;
            new->redirs = redir;
            new->pipe_next = NULL;
            new->next = NULL;
            new->next_type = type;

            if (!head)
                head = new;
            else if (type == TOKEN_PIPE)
                curr->pipe_next = new;
            else
                curr->next = new;

            curr = new;
            argv = NULL;
            redir = NULL;
            argc = 0;
        }
    }

    if (argv) {
        t_cmd *final = malloc(sizeof(t_cmd));
        final->argv = argv;
        final->redirs = redir;
        final->pipe_next = NULL;
        final->next = NULL;
        final->next_type = 0;
        if (!head)
            head = final;
        else
            curr->next = final;
    }
    return head;
}

void free_cmd(t_cmd *cmd) {
    while (cmd) {
        t_cmd *next = cmd->next;
        for (int i = 0; cmd->argv && cmd->argv[i]; i++)
            free(cmd->argv[i]);
        free(cmd->argv);

        t_redir *r = cmd->redirs;
        while (r) {
            t_redir *n = r->next;
            free(r->file);
            free(r);
            r = n;
        }
        free(cmd);
        cmd = next;
    }
}

void parse_and_execute(t_shell *shell, char *input) {
    char **tokens = lexer_tokenize(input);
    if (!tokens)
        return;
    printf("Tokens:\n");
    for (int i = 0; tokens[i]; i++)
        printf("  [%s]\n", tokens[i]);

    expand_variables(tokens, shell->env, shell->last_status);
    t_cmd *cmd = parse_tokens(tokens);
    t_cmd *tmp = cmd;
    while (tmp) {
        int i = 0, j = 0;
        while (tmp->argv[i]){
            printf("%s ", tmp->argv[i]);
            i++;
        }
        printf("\n");
        tmp = tmp->next;
    }
    free_tokens(tokens);
    if (!cmd) return;

    // execute_cmd(shell, cmd);
    free_cmd(cmd);
}