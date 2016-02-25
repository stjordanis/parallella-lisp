/* fl-device.c */
void pr(node *cell);
void addInt(long long i);
void addString(char *s);
void addValue(char *s, long long i);
char *scopy(char *s1, const char *s2);
long long stoi(const char *c);
int slen(char *s);
void prStats(void);
char *readFile(char *fileName);
void createFreelist(ememory *memory, int rows, int cols);
void createStringFreelist(ememory *memory, int rows, int cols);
void createNameFreelist(ememory *memory, int rows, int cols);
int coreID(unsigned int *row, unsigned int *col);
void coreInit(int argc, char *argv[]);
void nl(void);
void prpair(node *l);
void print(node *l);
void setflag(void);
string *smalloc(void);
string *string_malloc(void);
void string_free(string *n);
namestr *nmalloc(void);
namestr *name_malloc(void);
void name_free(namestr *n);
node *omalloc(void);
node *node_malloc(void);
void node_free(node *n);
void pushFree(stack *ptr, stack **stk);
stack *popFree(stack **stk);
void release_node(node *o);
node *newnode(enum ltype type);
node *sym(char *val);
node *cons(node *head, node *tail);
node *pair(node *head, node *tail);
node *func(node *(*fn)(node *, node *), enum ltype type);
node *lambda(node *args, node *sexp);
node *integer(long long num);
node *newcontext(node *bindings, node *top);
void clear_bindings(node *env);
node *lastcell(node *list);
node *append(node *list, node *obj);
node *concat(node *l1, node *l2);
void atl(node **l, node *item);
void add_pair(node *head, node *tail, node **list);
void pushNode(node *item, node **stk);
node *popNode(node **stk);
node *nextarg(node **pargs);
char *name(node *o);
int strequal(char *s1, char *s2);
node *assq(char *key, node *list);
node *lookupsym(char *name, node *env);
node *make_env(node *vars, node *vals, node *env);
node *el_car(node *args, node *env);
node *el_cdr(node *args, node *env);
node *el_nilp(node *args, node *env);
node *el_quote(node *args, node *env);
node *el_cons(node *args, node *env);
node *el_cond(node *args, node *env);
node *el_if(node *args, node *env);
node *el_lambda(node *args, node *env);
node *el_label(node *args, node *env);
node *el_ldefine(node *args, node *env);
node *el_loop(node *args, node *env);
node *el_block(node *args, node *env);
node *el_progn(node *args, node *env);
node *el_print(node *args, node *env);
node *el_terpri(node *args, node *env);
node *binary(node *args, int fcn);
node *compare(node *args, int fcn);
node *el_lessthan(node *args, node *env);
node *el_greaterthan(node *args, node *env);
node *el_eq(node *args, node *env);
node *el_plus(node *args, node *env);
node *el_minus(node *args, node *env);
node *el_times(node *args, node *env);
node *el_divide(node *args, node *env);
node *el_atom(node *args, node *env);
node *el_equal(node *args, node *env);
node *el_lessthanequal(node *args, node *env);
node *el_greaterthanequal(node *args, node *env);
node *el_defun(node *args, node *env);
node *el_consp(node *args, node *env);
node *el_funcall(node *args, node *env);
node *el_zerop(node *args, node *env);
node *el_sub1(node *args, node *env);
node *el_add1(node *args, node *env);
node *el_numberp(node *args, node *env);
node *el_or(node *args, node *env);
node *el_and(node *args, node *env);
node *el_not(node *args, node *env);
node *el_setflag(node *args, node *env);
node *el_id(node *args, node *env);
node *init_lisp(void);
int getChar(char **s);
int ungetChar(char **s);
char *getToken(char **s, char *token);
node *tokenize(char **code);
int equal(node *sym, char *s2);
int is_valid_int(char *str);
node *makeNode(node *n);
node *_parse(node **code, char *terminator);
node *parse(node **code);
node *parse_string(char **input);
int length(node *l);
node *evlambda(node *vals, node *expr, node *env);
node *evform(node *fnode, node *exp, node *env);
node *evsym(node *exp, node *env);
node *eval_list(node *sexp, node *env);
node *eval(node *input, node *env);
void REPL(char *input);
int main(int argc, char *argv[]);
