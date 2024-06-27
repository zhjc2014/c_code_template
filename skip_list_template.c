#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// 跳表的最大层数
#define SKIPLIST_MAX_LEVEL 16

// 定义跳表数据类型和操作的宏
#define DEFINE_SKIPLIST_TYPE(type, prefix)                                \
typedef struct prefix##_node {                                            \
    int key;                                                              \
    type value;                                                           \
    struct prefix##_node *forward[];                                       \
} prefix##_node;                                                          \
                                                                          \
typedef struct {                                                          \
    int level;                                                            \
    prefix##_node *header;                                                 \
} prefix##_skiplist;                                                      \
                                                                          \
static prefix##_node* prefix##_create_node(int level, int key, type value) { \
    prefix##_node *node = (prefix##_node *)malloc(sizeof(prefix##_node) + level * sizeof(prefix##_node *)); \
    node->key = key;                                                      \
    node->value = value;                                                  \
    for (int i = 0; i < level; i++) {                                     \
        node->forward[i] = NULL;                                          \
    }                                                                     \
    return node;                                                          \
}                                                                         \
                                                                          \
static prefix##_skiplist* prefix##_create_skiplist() {                    \
    prefix##_skiplist *list = (prefix##_skiplist *)malloc(sizeof(prefix##_skiplist));   \
    list->level = 1;                                                      \
    list->header = prefix##_create_node(SKIPLIST_MAX_LEVEL, INT_MAX, 0);  \
    return list;                                                          \
}                                                                         \
                                                                          \
static void prefix##_skiplist_insert(prefix##_skiplist *list, int key, type value) {   \
    prefix##_node *update[SKIPLIST_MAX_LEVEL];                            \
    prefix##_node *current = list->header;                                \
                                                                          \
    for (int i = list->level - 1; i >= 0; i--) {                          \
        while (current->forward[i] && current->forward[i]->key < key) {   \
            current = current->forward[i];                                \
        }                                                                 \
        update[i] = current;                                              \
    }                                                                     \
                                                                          \
    int level = random_level();                                           \
    if (level > list->level) {                                            \
        for (int i = list->level; i < level; i++) {                       \
            update[i] = list->header;                                     \
        }                                                                 \
        list->level = level;                                              \
    }                                                                     \
                                                                          \
    prefix##_node *new_node = prefix##_create_node(level, key, value);    \
    for (int i = 0; i < level; i++) {                                     \
        new_node->forward[i] = update[i]->forward[i];                     \
        update[i]->forward[i] = new_node;                                 \
    }                                                                     \
}                                                                         \
                                                                          \
static int prefix##_skiplist_search(prefix##_skiplist *list, int key, type *value) {  \
    prefix##_node *current = list->header;                                \
    for (int i = list->level - 1; i >= 0; i--) {                          \
        while (current->forward[i] && current->forward[i]->key < key) {   \
            current = current->forward[i];                                \
        }                                                                 \
    }                                                                     \
    current = current->forward[0];                                        \
                                                                          \
    if (current && current->key == key) {                                 \
        *value = current->value;                                          \
        return 1;                                                         \
    }                                                                     \
    return 0;                                                             \
}                                                                         \
                                                                          \
static void prefix##_skiplist_delete(prefix##_skiplist *list, int key) {  \
    prefix##_node *update[SKIPLIST_MAX_LEVEL];                            \
    prefix##_node *current = list->header;                                \
                                                                          \
    for (int i = list->level - 1; i >= 0; i--) {                          \
        while (current->forward[i] && current->forward[i]->key < key) {   \
            current = current->forward[i];                                \
        }                                                                 \
        update[i] = current;                                              \
    }                                                                     \
                                                                          \
    current = current->forward[0];                                        \
                                                                          \
    if (current && current->key == key) {                                 \
        for (int i = 0; i < list->level; i++) {                           \
            if (update[i]->forward[i] != current) {                       \
                break;                                                    \
            }                                                             \
            update[i]->forward[i] = current->forward[i];                  \
        }                                                                 \
        free(current);                                                    \
                                                                          \
        while (list->level > 1 && list->header->forward[list->level - 1] == NULL) { \
            list->level--;                                                \
        }                                                                 \
    }                                                                     \
}

static int random_level() {
    int level = 1;
    while (rand() % 2 == 0 && level < SKIPLIST_MAX_LEVEL) {
        level++;
    }
    return level;
}

// 使用宏实例化不同类型的跳表
DEFINE_SKIPLIST_TYPE(int, int)
DEFINE_SKIPLIST_TYPE(float, float)
DEFINE_SKIPLIST_TYPE(char*, string)

// 示例程序
int main() {
    srand((unsigned)time(NULL));

    // 创建一个整数类型的跳表
    int_skiplist *ilist = int_create_skiplist();

    int_skiplist_insert(ilist, 3, 30);
    int_skiplist_insert(ilist, 6, 60);
    int_skiplist_insert(ilist, 7, 70);
    int_skiplist_insert(ilist, 9, 90);
    int_skiplist_insert(ilist, 12, 120);
    int_skiplist_insert(ilist, 19, 190);

    int value;
    if (int_skiplist_search(ilist, 7, &value)) {
        printf("Found key 7 with value %d\n", value);
    } else {
        printf("Key 7 not found\n");
    }

    int_skiplist_delete(ilist, 7);

    if (int_skiplist_search(ilist, 7, &value)) {
        printf("Found key 7 with value %d\n", value);
    } else {
        printf("Key 7 not found\n");
    }

    // 创建一个浮点数类型的跳表
    float_skiplist *flist = float_create_skiplist();

    float_skiplist_insert(flist, 3, 30.1f);
    float_skiplist_insert(flist, 6, 60.2f);
    float_skiplist_insert(flist, 7, 70.3f);
    float_skiplist_insert(flist, 9, 90.4f);
    float_skiplist_insert(flist, 12, 120.5f);
    float_skiplist_insert(flist, 19, 190.6f);

    float fvalue;
    if (float_skiplist_search(flist, 7, &fvalue)) {
        printf("Found key 7 with value %f\n", fvalue);
    } else {
        printf("Key 7 not found\n");
    }

    float_skiplist_delete(flist, 7);

    if (float_skiplist_search(flist, 7, &fvalue)) {
        printf("Found key 7 with value %f\n", fvalue);
    } else {
        printf("Key 7 not found\n");
    }

    // 创建一个字符串类型的跳表
    string_skiplist *slist = string_create_skiplist();

    string_skiplist_insert(slist, 3, "thirty");
    string_skiplist_insert(slist, 6, "sixty");
    string_skiplist_insert(slist, 7, "seventy");
    string_skiplist_insert(slist, 9, "ninety");
    string_skiplist_insert(slist, 12, "one twenty");
    string_skiplist_insert(slist, 19, "one ninety");

    char *svalue;
    if (string_skiplist_search(slist, 7, &svalue)) {
        printf("Found key 7 with value %s\n", svalue);
    } else {
        printf("Key 7 not found\n");
    }

    string_skiplist_delete(slist, 7);

    if (string_skiplist_search(slist, 7, &svalue)) {
        printf("Found key 7 with value %s\n", svalue);
    } else {
        printf("Key 7 not found\n");
    }

    return 0;
}
