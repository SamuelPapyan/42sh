#include "42sh.h"

static int is_operator_char(char c) {
    return (c == '|' || c == '&' || c == ';' || c == '<' || c == '>');
}

static int is_separator(const char *s) {
    return (!strncmp(s, "&&", 2) || !strncmp(s, "||", 2)
        || !strncmp(s, ">>", 2) || !strncmp(s, "<<", 2)
        || *s == ';' || *s == '|' || *s == '<' || *s == '>');
}

static char *strdup_until(const char *start, const char *end) {
    size_t  len = end - start;
    char    *res = malloc(len + 1);
    if (!res)
        return NULL;
    memcpy(res, start, len);
    res[len] = '\0';
    return res;
}

static char *read_word(const char **s) {
    const char  *start = *s;
    char        quote = 0;

    if (**s== '\'' || **s == '"')
        quote = *((*s)++);

    while (**s) {
        if (quote && **s == quote) {
            (*s)++;
            break;
        }
        if (!quote && (isspace(**s) || is_operator_char(**s)))
            break;
        (*s)++;
    }
    return strdup_until(start, *s);
}

static size_t   count_tokens(const char *s) {
    size_t  count = 0;
    while (*s) {
        while (isspace(*s)) s++;
        if (!*s) break;
        if (is_separator(s))
        {
            if (!strncmp(s, "&&", 2) || !strncmp(s, "||", 2)
                || !strncmp(s, ">>", 2) || !strncmp(s, "<<", 2))
                s += 2;
            else
                s += 1;
        }
        else {
            if (*s == '"' || *s == '\'') {
                char quote = *s++;
                while (*s && *s != quote) s++;
                if (*s) s++;
            }
            else {
                while (*s && !isspace(*s) && !is_operator_char(*s)) s++;
            }
        }
        count++;
    }
    return count;
}

char **lexer_tokenize(const char *input) {
    char **tokens;
    size_t i = 0;
    size_t count = count_tokens(input);

    tokens = malloc(sizeof(char *) * (count + 1));
    if (!tokens)
        return NULL;

    while (*input) {
        while (isspace(*input)) input++;
        if (!*input) break;

        if (is_separator(input)) {
            if (!strncmp(input, "&&", 2) || !strncmp(input, "||", 2)
                || !strncmp(input, ">>", 2) || !strncmp(input, "<<", 2)) {
                tokens[i++] = strndup(input, 2);
                input += 2;
            } else
                tokens[i++] = strndup(input++, 1);
        }
        else
            tokens[i++] = read_word(&input);
    }
    tokens[i] = NULL;
    return tokens;
}

void free_tokens(char **tokens) {
    size_t i = 0;
    if (!tokens) return;
    while (tokens[i])
        free(tokens[i++]);
    free(tokens);
}