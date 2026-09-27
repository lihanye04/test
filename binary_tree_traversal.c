#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 1000

/* 二叉树节点：指针连接结构 */
typedef struct Node {
    char data;
    struct Node *left;
    struct Node *right;
} Node;

/* 栈结构，用于非递归遍历和括号解析 */
typedef struct {
    Node *items[MAX];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, Node *n) {
    if (s->top >= MAX - 1) {
        fprintf(stderr, "Stack overflow\n");
        exit(1);
    }
    s->items[++(s->top)] = n;
}

Node *pop(Stack *s) {
    if (isEmpty(s)) return NULL;
    return s->items[(s->top)--];
}

/* 创建新节点 */
Node *createNode(char data) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (n == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    return n;
}

/* 括号解析辅助栈 */
typedef struct {
    Node *node;
    int nextIsRight;   /* 0: 下一个子节点放左边, 1: 下一个子节点放右边 */
} ParseFrame;

/*
 * 解析括号表示法二叉树
 * 例如：A(B(D(H,I),E(,J)),C(F,G))
 * 成功返回根节点，失败时 *ok = 0
 */
Node *parseTree(const char *input, int *ok) {
    ParseFrame stack[MAX];
    int top = -1;
    Node *root = NULL;
    Node *last = NULL;
    int i = 0;
    int len = (int)strlen(input);

    *ok = 1;

    while (i < len) {
        char c = input[i];

        if (isspace((unsigned char)c)) {
            i++;
            continue;
        }

        if (c == '(') {
            if (last == NULL) {
                *ok = 0;
                return NULL;
            }
            if (top >= MAX - 1) {
                *ok = 0;
                return NULL;
            }
            top++;
            stack[top].node = last;
            stack[top].nextIsRight = 0;
            i++;
        } else if (c == ',') {
            if (top < 0) {
                *ok = 0;
                return NULL;
            }
            stack[top].nextIsRight = 1;
            i++;
        } else if (c == ')') {
            if (top < 0) {
                *ok = 0;
                return NULL;
            }
            top--;
            i++;
        } else {
            /* 节点数据，按单字符处理，例如 A, B, C */
            char data = c;
            Node *node = createNode(data);

            if (top < 0) {
                if (root != NULL) {
                    *ok = 0;
                    return NULL;
                }
                root = node;
            } else {
                Node *parent = stack[top].node;

                if (stack[top].nextIsRight == 0) {
                    if (parent->left != NULL) {
                        *ok = 0;
                        return NULL;
                    }
                    parent->left = node;
                    stack[top].nextIsRight = 1;
                } else {
                    if (parent->right != NULL) {
                        *ok = 0;
                        return NULL;
                    }
                    parent->right = node;
                }
            }

            last = node;
            i++;
        }
    }

    if (top != -1) {
        *ok = 0;
        return NULL;
    }

    if (root == NULL) {
        *ok = 0;
        return NULL;
    }

    return root;
}

/* 非递归前序遍历：Root -> Left -> Right */
void preorder(Node *root) {
    if (root == NULL) {
        printf("\n");
        return;
    }

    Stack s;
    initStack(&s);
    push(&s, root);

    while (!isEmpty(&s)) {
        Node *n = pop(&s);
        printf("%c ", n->data);

        /* 栈是后进先出，所以先压右子，再压左子 */
        if (n->right != NULL) push(&s, n->right);
        if (n->left != NULL) push(&s, n->left);
    }

    printf("\n");
}

/* 非递归中序遍历：Left -> Root -> Right */
void inorder(Node *root) {
    Stack s;
    initStack(&s);

    Node *curr = root;

    while (curr != NULL || !isEmpty(&s)) {
        while (curr != NULL) {
            push(&s, curr);
            curr = curr->left;
        }

        curr = pop(&s);
        printf("%c ", curr->data);
        curr = curr->right;
    }

    printf("\n");
}

/* 非递归后序遍历：Left -> Right -> Root */
void postorder(Node *root) {
    if (root == NULL) {
        printf("\n");
        return;
    }

    Stack s1, s2;
    initStack(&s1);
    initStack(&s2);

    push(&s1, root);

    while (!isEmpty(&s1)) {
        Node *n = pop(&s1);
        push(&s2, n);

        if (n->left != NULL) push(&s1, n->left);
        if (n->right != NULL) push(&s1, n->right);
    }

    while (!isEmpty(&s2)) {
        Node *n = pop(&s2);
        printf("%c ", n->data);
    }

    printf("\n");
}

int main(void) {
    char input[1024];

    printf("Enter binary tree in bracket notation:\n");
    printf("Example: A(B(D(H,I),E(,J)),C(F,G))\n> ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error.\n");
        return 1;
    }

    /* 去掉末尾换行 */
    input[strcspn(input, "\n")] = '\0';

    int ok = 0;
    Node *root = parseTree(input, &ok);

    if (!ok) {
        printf("Error: Invalid bracket notation.\n");
        return 1;
    }

    printf("\nInput tree: %s\n", input);
    printf("Preorder  : ");
    preorder(root);

    printf("Inorder   : ");
    inorder(root);

    printf("Postorder : ");
    postorder(root);

    return 0;
}
