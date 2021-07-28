#include <malloc.h>
#include <assert.h>

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
    jj_node_type__unnamed_string,
} jj_node_type;

typedef enum {
    jj_keyword_value__false = 0,
    jj_keyword_value__true = 1,
    jj_keyword_value__null = -1
} jj_keyword_value;


typedef struct {
    char *name;
} jj_object_node;

typedef struct {
    char *name;
} jj_array_node;

typedef struct {
    char *name;
    int value;
} jj_number_node;

typedef struct {
    char *name;
    char *value;
} jj_string_node;

typedef struct {
    char *value;
} jj_unnamed_string_node;

typedef struct {
    char *name;
    jj_keyword_value value;
} jj_keyword_node;


typedef struct {
    jj_node_type type;
    int parent;
    int level;
    int *children;
    int children_count;
    int children_capacity;
    union {
        jj_object_node object;
        jj_array_node array;
        jj_number_node number;
        jj_string_node string;
        jj_unnamed_string_node unnamed_string;
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
        if (ctx->nodes_capacity) {
            size_t alloc_size = ctx->nodes_capacity * 2;
            jj_node *old_memory = ctx->nodes;
            ctx->nodes = calloc(alloc_size, sizeof(jj_node));
            ctx->nodes_capacity = alloc_size;
            memcpy_s(ctx->nodes, alloc_size * sizeof(jj_node), old_memory,
                     ctx->nodes_count * sizeof(jj_node));
            free(old_memory);
        } else {
            size_t alloc_size = 16;
            ctx->nodes = calloc(alloc_size, sizeof(jj_node));
            ctx->nodes_capacity = alloc_size;
        }
    }


    if (ctx->active_stack_capacity < ctx->active_stack_count + 1) {
        if (ctx->active_stack_capacity) {
            size_t alloc_size = ctx->active_stack_capacity * 2;
            int *old_memory = ctx->active_stack;
            ctx->active_stack = calloc(alloc_size, sizeof(int));
            ctx->active_stack_capacity = alloc_size;
            memcpy_s(ctx->active_stack, alloc_size * sizeof(int), old_memory,
                     ctx->active_stack_count * sizeof(int));
            free(old_memory);
        } else {
            size_t alloc_size = 16;
            ctx->active_stack = calloc(alloc_size, sizeof(int));
            ctx->active_stack_capacity = alloc_size;
        }
    }
}


jj_add_child_node(jj_context *ctx, int parent_id, int child_id) {
    jj_node * parent_node = &ctx->nodes[parent_id];

    if (parent_node->children_capacity < parent_node->children_count + 1) {
        if (parent_node->children_capacity) {
            size_t alloc_size = parent_node->children_capacity * 2;
            int *old_memory = parent_node->children;
            parent_node->children = calloc(alloc_size, sizeof(int));
            parent_node->children_capacity = alloc_size;
            memcpy_s(parent_node->children, alloc_size * sizeof(int), old_memory,
                     parent_node->children_count * sizeof(int));
            free(old_memory);
        } else {
            size_t alloc_size = 16;
            parent_node->children = calloc(alloc_size, sizeof(int));
            parent_node->children_capacity = alloc_size;
        }
    }

    parent_node->children[parent_node->children_count] = child_id;
    ++parent_node->children_count;
}


void jj_root_begin(jj_context *ctx) {
    jj_context_allocate(ctx);

    ctx->nodes[ctx->nodes_count] = (jj_node){0};
    ctx->nodes[ctx->nodes_count].parent = -1;
    ctx->nodes[ctx->nodes_count].level = ctx->active_stack_count;
    ctx->nodes[ctx->nodes_count].type = jj_node_type__root;

    ctx->active_stack[ ctx->active_stack_count++ ] = ctx->nodes_count++;
}


void jj_root_end(jj_context *ctx) {
    --ctx->active_stack_count;
}

void jj_object_begin(jj_context *ctx, char *name) {
    jj_context_allocate(ctx);

    ctx->nodes[ctx->nodes_count] = (jj_node){0};

    int parent_id = ctx->active_stack[ctx->active_stack_count - 1];
    ctx->nodes[ctx->nodes_count].parent = parent_id;
    ctx->nodes[ctx->nodes_count].level = ctx->active_stack_count;

    ctx->nodes[ctx->nodes_count].type = jj_node_type__object;
    ctx->nodes[ctx->nodes_count].object.name = name;

    jj_add_child_node(ctx, parent_id, ctx->nodes_count);

    ctx->active_stack[ ctx->active_stack_count++ ] = ctx->nodes_count++;
}


void jj_object_end(jj_context *ctx) {
    --ctx->active_stack_count;
}


void jj_array_begin(jj_context *ctx, char *name) {
    jj_context_allocate(ctx);

    ctx->nodes[ctx->nodes_count] = (jj_node){0};

    int parent_id = ctx->active_stack[ctx->active_stack_count - 1];
    ctx->nodes[ctx->nodes_count].parent = parent_id;
    ctx->nodes[ctx->nodes_count].level = ctx->active_stack_count;

    ctx->nodes[ctx->nodes_count].type = jj_node_type__array;
    ctx->nodes[ctx->nodes_count].array.name = name;

    jj_add_child_node(ctx, parent_id, ctx->nodes_count);

    ctx->active_stack[ ctx->active_stack_count++ ] = ctx->nodes_count++;
}


void jj_array_end(jj_context *ctx) {
    --ctx->active_stack_count;
}



void jj_number(jj_context *ctx, char *name, int value) {
    jj_context_allocate(ctx);

    ctx->nodes[ctx->nodes_count] = (jj_node){0};

    int parent_id = ctx->active_stack[ctx->active_stack_count - 1];
    ctx->nodes[ctx->nodes_count].parent = parent_id;
    ctx->nodes[ctx->nodes_count].level = ctx->active_stack_count;

    ctx->nodes[ctx->nodes_count].type = jj_node_type__number;
    ctx->nodes[ctx->nodes_count].number.name = name;
    ctx->nodes[ctx->nodes_count].number.value = value;

    jj_add_child_node(ctx, parent_id, ctx->nodes_count);

    ++ctx->nodes_count;
}


void jj_string(jj_context *ctx, char *name, char *value) {
    jj_context_allocate(ctx);

    ctx->nodes[ctx->nodes_count] = (jj_node){0};

    int parent_id = ctx->active_stack[ctx->active_stack_count - 1];
    ctx->nodes[ctx->nodes_count].parent = parent_id;
    ctx->nodes[ctx->nodes_count].level = ctx->active_stack_count;

    ctx->nodes[ctx->nodes_count].type = jj_node_type__string;
    ctx->nodes[ctx->nodes_count].string.name = name;
    ctx->nodes[ctx->nodes_count].string.value = value;

    jj_add_child_node(ctx, parent_id, ctx->nodes_count);

    ++ctx->nodes_count;
}

void jj_unnamed_string(jj_context *ctx, char *value) {
    jj_context_allocate(ctx);

    ctx->nodes[ctx->nodes_count] = (jj_node){0};

    int parent_id = ctx->active_stack[ctx->active_stack_count - 1];
    ctx->nodes[ctx->nodes_count].parent = parent_id;
    ctx->nodes[ctx->nodes_count].level = ctx->active_stack_count;

    ctx->nodes[ctx->nodes_count].type = jj_node_type__unnamed_string;
    ctx->nodes[ctx->nodes_count].unnamed_string.value = value;

    jj_add_child_node(ctx, parent_id, ctx->nodes_count);

    ++ctx->nodes_count;
}



#define MERGE_(a,b)  a##b
#define LABEL_(a) MERGE_(unique_name_, a)
#define LINEVAR LABEL_(__LINE__)

#define jj_root(x) jj_root_begin(x); \
        for (int LINEVAR = 0; LINEVAR < 1; ++LINEVAR, jj_root_end(x))
#define jj_object(x, name) jj_object_begin(x, name); \
        for (int LINEVAR = 0; LINEVAR < 1; ++LINEVAR, jj_object_end(x))
#define jj_array(x, name) jj_array_begin(x, name); \
        for (int LINEVAR = 0; LINEVAR < 1; ++LINEVAR, jj_array_end(x))


void jj_new_line(jj_context *ctx, char *ret, int index) {
    jj_node *node = &ctx->nodes[index];

    if (node->parent < 0)
        return;

    if (ctx->nodes[node->parent].children[0] == index) {
        sprintf(ret, "%s\n", ret);
    } else {
        sprintf(ret, "%s,\n", ret);
    }

    for (int i = 0; i < node->level; ++i)
        sprintf(ret, "%s  ", ret);
}


char *jj_serialize_node(jj_context *ctx, char *ret, int index) {
    jj_node *node = &ctx->nodes[index];

    jj_new_line(ctx, ret, index);

    if (node->type == jj_node_type__root) {
        sprintf(ret, "%s{", ret);
        for (int i = 0; i < node->children_count; ++i)
            ret = jj_serialize_node(ctx, ret, node->children[i]);

        sprintf(ret, "%s\n", ret);
        for (int i = 0; i < node->level; ++i)
            sprintf(ret, "%s  ", ret);
        sprintf(ret, "%s}", ret);
        
    } else if (node->type == jj_node_type__object) {
        sprintf(ret, "%s\"%s\": {", ret, node->object.name);
        for (int i = 0; i < node->children_count; ++i)
            ret = jj_serialize_node(ctx, ret, node->children[i]);

        sprintf(ret, "%s\n", ret);
        for (int i = 0; i < node->level; ++i)
            sprintf(ret, "%s  ", ret);
        sprintf(ret, "%s}", ret);

    } else if (node->type == jj_node_type__array) {
        sprintf(ret, "%s\"%s\": [", ret, node->object.name);
        for (int i = 0; i < node->children_count; ++i)
            ret = jj_serialize_node(ctx, ret, node->children[i]);

        sprintf(ret, "%s\n", ret);
        for (int i = 0; i < node->level; ++i)
            sprintf(ret, "%s  ", ret);
        sprintf(ret, "%s]", ret);

    } else if (node->type == jj_node_type__string) {
        sprintf(ret, "%s\"%s\": \"%s\"", ret, node->string.name, node->string.value);
    } else if (node->type == jj_node_type__unnamed_string) {
        sprintf(ret, "%s\"%s\"", ret, node->string.name, node->unnamed_string.value);
    } else if (node->type == jj_node_type__number) {
        sprintf(ret, "%s\"%s\": %d", ret, node->number.name, node->number.value);
    } else {
        sprintf(ret, "%s[node]", ret);
    }


    return ret;
}

char *jj_serialize(jj_context *ctx) {
    size_t ret_capacity = 1024;
    char *ret = calloc(ret_capacity, 1);

    ret = jj_serialize_node(ctx, ret, 0);

    return ret;
}
