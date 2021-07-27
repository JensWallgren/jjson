#include <malloc.h>

typedef enum {
    jj_node_type__null = 0,
    jj_node_type__root = 1,
    jj_node_type__object,
    jj_node_type__array,
    jj_node_type__field,
    jj_node_type__string,
    jj_node_type__number,
    jj_node_type__keyword,
    jj_node_type__value,
} jj_node_type;

typedef enum {
    jj_keyword_value__false = 0,
    jj_keyword_value__true = 1,
    jj_keyword_value__null = -1
} jj_keyword_value;

typedef struct {
    int *children;
    int children_count;
} jj_root_node;

typedef struct {
    char *name;
    int *children;
    int children_count;
} jj_object_node;

typedef struct {
    char *name;
    int value;
} jj_number_node;

typedef struct {
    char *name;
    char *value;
} jj_string_node;

typedef struct {
    char *name;
    jj_keyword_value value;
} jj_keyword_node;


typedef struct {
    jj_node_type type;
    union {
        jj_root_node root;
        jj_object_node object;
        jj_number_node number;
        jj_string_node string;
    };
} jj_node;



typedef struct {
    jj_node *nodes;
    int nodes_capacity;
    int nodes_count;

    int *active_stack;
    int active_stack_capacity;
    int active_stack_count;
} jj_context;

void jj_context_allocate(jj_context *ctx) {
    if (ctx->nodes_capacity < ctx->nodes_count + 1) {
        size_t alloc_size = ctx->nodes_capacity ? ctx->nodes_capacity * 2 : 16;
        jj_node *old_memory = ctx->nodes;
        ctx->nodes = malloc(alloc_size * sizeof(jj_node));
        ctx->nodes_capacity = alloc_size;
        memcpy_s(ctx->nodes, alloc_size * sizeof(jj_node), old_memory,
                 ctx->nodes_count * sizeof(jj_node));
        free(ctx->nodes);
    }


    if (ctx->active_stack_capacity < ctx->active_stack_count + 1) {
        size_t alloc_size = ctx->active_stack_capacity ? ctx->active_stack_capacity * 2 : 16;
        int *old_memory = ctx->active_stack;
        ctx->active_stack = malloc(alloc_size);
        ctx->active_stack_capacity = alloc_size;
        memcpy_s(ctx->active_stack, alloc_size * sizeof(int), old_memory,
                 ctx->active_stack_count * sizeof(int));
        free(ctx->active_stack);
    }
}

void jj_root_begin(jj_context *ctx) {
    jj_context_allocate(ctx);

    ctx->nodes[ctx->nodes_count] = (jj_node){0};
    ctx->nodes[ctx->nodes_count].type = jj_node_type__root;

    ctx->active_stack[ ++ctx->active_stack_count ] = ++ctx->nodes_count;
}


void jj_root_end(jj_context *ctx) {
    --ctx->active_stack_count;
}


void jj_number(jj_context *ctx, char *field_name, int value) {
    jj_context_allocate(ctx);

    ctx->nodes[ctx->nodes_count] = (jj_node){0};
    ctx->nodes[ctx->nodes_count].type = jj_node_type__number;
    ctx->nodes[ctx->nodes_count].number.name = field_name;
    ctx->nodes[ctx->nodes_count].number.value = value;

    ++ctx->nodes_count;
}

