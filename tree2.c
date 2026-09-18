#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000  /* 最大节点数，防止栈溢出警告 */

/* ===================== 指针连接结构 ===================== */
typedef struct Node {
    char data;
    struct Node *left;
    struct Node *right;
} Node;

Node* newNode(char d) {
    Node* p = (Node*)malloc(sizeof(Node));
    if (p == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    p->data = d;
    p->left = NULL;
    p->right = NULL;
    return p;
}

/* 括号表示法 -> 指针二叉树 */
Node* parsePtr(const char* s, int* pos) {
    Node* node;
    char val;

    if (s[*pos] == '\0' || s[*pos] == ',' || s[*pos] == ')')
        return NULL;

    val = s[*pos];
    (*pos)++;
    node = newNode(val);

    if (s[*pos] == '(') {
        (*pos)++;
        if (s[*pos] != ',' && s[*pos] != ')')
            node->left = parsePtr(s, pos);
        if (s[*pos] == ',') {
            (*pos)++;
            if (s[*pos] != ')' && s[*pos] != '\0')
                node->right = parsePtr(s, pos);
        }
        if (s[*pos] == ')') (*pos)++;
    }
    return node;
}

/* 括号表示法 -> 数组二叉树 */
void parseArr(const char* s, int* pos, char* a, int idx) {
    char val;
    if (idx >= MAX) return;
    if (s[*pos] == '\0' || s[*pos] == ',' || s[*pos] == ')')
        return;

    val = s[*pos];
    (*pos)++;
    a[idx] = val;

    if (s[*pos] == '(') {
        (*pos)++;
        if (s[*pos] != ',' && s[*pos] != ')')
            parseArr(s, pos, a, idx * 2);
        if (s[*pos] == ',') {
            (*pos)++;
            if (s[*pos] != ')' && s[*pos] != '\0')
                parseArr(s, pos, a, idx * 2 + 1);
        }
        if (s[*pos] == ')') (*pos)++;
    }
}

/* ===================== 二叉树输出（向左倒） ===================== */
void printPtr(Node* r, int depth) {
    int i;
    if (!r) return;
    printPtr(r->right, depth + 1);
    for (i = 0; i < depth; i++) printf("    ");
    printf("%c\n", r->data);
    printPtr(r->left, depth + 1);
}

void printArr(char* a, int idx, int depth) {
    int i;
    if (idx >= MAX || a[idx] == 0) return;
    printArr(a, idx * 2 + 1, depth + 1);
    for (i = 0; i < depth; i++) printf("    ");
    printf("%c\n", a[idx]);
    printArr(a, idx * 2, depth + 1);
}

/* ===================== 树信息 ===================== */
int countPtr(Node* r) {
    if (!r) return 0;
    return 1 + countPtr(r->left) + countPtr(r->right);
}

int leafPtr(Node* r) {
    if (!r) return 0;
    if (!r->left && !r->right) return 1;
    return leafPtr(r->left) + leafPtr(r->right);
}

int heightPtr(Node* r) {
    int lh, rh;
    if (!r) return 0;
    lh = heightPtr(r->left);
    rh = heightPtr(r->right);
    return 1 + (lh > rh ? lh : rh);
}

int degreePtr(Node* r) {
    int c, d1, d2;
    if (!r) return 0;
    c = (r->left ? 1 : 0) + (r->right ? 1 : 0);
    d1 = degreePtr(r->left);
    d2 = degreePtr(r->right);
    if (d1 > c) c = d1;
    if (d2 > c) c = d2;
    return c;
}

int maxIdxArr(char* a) {
    int i;
    for (i = MAX - 1; i >= 1; i--)
        if (a[i]) return i;
    return 0;
}

int countArr(char* a) {
    int i, c = 0;
    for (i = 1; i < MAX; i++) if (a[i]) c++;
    return c;
}

int leafArr(char* a) {
    int i, c = 0;
    for (i = 1; i < MAX; i++) {
        if (!a[i]) continue;
        if ((i * 2 >= MAX || !a[i * 2]) && (i * 2 + 1 >= MAX || !a[i * 2 + 1])) c++;
    }
    return c;
}

int heightArr(char* a) {
    int m = maxIdxArr(a), h = 0;
    while (m > 0) { h++; m /= 2; }
    return h;
}

int degreeArr(char* a) {
    int i, c, d = 0;
    for (i = 1; i < MAX; i++) {
        if (!a[i]) continue;
        c = 0;
        if (i * 2 < MAX && a[i * 2]) c++;
        if (i * 2 + 1 < MAX && a[i * 2 + 1]) c++;
        if (c > d) d = c;
    }
    return d;
}

/* ===================== 形态判别 ===================== */
int isCompletePtr(Node* r) {
    Node* q[MAX];
    int idx[MAX];
    int front = 0, back = 0;
    int n = 0, maxi = 0;

    if (!r) return 1;
    q[back] = r; idx[back] = 1; back++;

    while (front < back) {
        Node* cur = q[front];
        int i = idx[front];
        front++;
        n++;
        if (i > maxi) maxi = i;
        if (cur->left)  { q[back] = cur->left;  idx[back] = i * 2;     back++; }
        if (cur->right) { q[back] = cur->right; idx[back] = i * 2 + 1; back++; }
    }
    return n == maxi;
}

int isFullPtr(Node* r) {
    if (!r) return 1;
    if (!r->left && !r->right) return 1;
    if (r->left && r->right) return isFullPtr(r->left) && isFullPtr(r->right);
    return 0;
}

int isSkewedPtr(Node* r) {
    int c;
    if (!r) return 1;
    c = (r->left ? 1 : 0) + (r->right ? 1 : 0);
    if (c > 1) return 0;
    return isSkewedPtr(r->left) && isSkewedPtr(r->right);
}

int isCompleteArr(char* a) {
    return countArr(a) == maxIdxArr(a);
}

int isFullArr(char* a) {
    int n = countArr(a);
    int h = heightArr(a);
    if (h > 20) return 0;
    return n == (1 << h) - 1;
}

int isSkewedArr(char* a) {
    int i, c;
    for (i = 1; i < MAX; i++) {
        if (!a[i]) continue;
        c = 0;
        if (i * 2 < MAX && a[i * 2]) c++;
        if (i * 2 + 1 < MAX && a[i * 2 + 1]) c++;
        if (c > 1) return 0;
    }
    return 1;
}

/* ===================== 子节点、父节点、兄弟节点 ===================== */
Node* findPtr(Node* r, char v) {
    Node* p;
    if (!r) return NULL;
    if (r->data == v) return r;
    p = findPtr(r->left, v);
    if (p) return p;
    return findPtr(r->right, v);
}

Node* parentPtr(Node* r, char v) {
    Node* p;
    if (!r) return NULL;
    if ((r->left && r->left->data == v) || (r->right && r->right->data == v)) return r;
    p = parentPtr(r->left, v);
    if (p) return p;
    return parentPtr(r->right, v);
}

void relationsPtr(Node* root, char v) {
    Node* node = findPtr(root, v);
    Node* par;
    if (!node) { printf("Node %c not found\n", v); return; }

    printf("Node %c:\n", v);
    printf("  Left child  : ");
    if (node->left)  printf("%c\n", node->left->data);  else printf("None\n");
    printf("  Right child : ");
    if (node->right) printf("%c\n", node->right->data); else printf("None\n");

    par = parentPtr(root, v);
    printf("  Parent      : ");
    if (par) printf("%c\n", par->data); else printf("None\n");

    printf("  Sibling     : ");
    if (par) {
        if (par->left == node && par->right)       printf("%c\n", par->right->data);
        else if (par->right == node && par->left)  printf("%c\n", par->left->data);
        else printf("None\n");
    } else printf("None\n");
}

int findArr(char* a, char v) {
    int i;
    for (i = 1; i < MAX; i++) if (a[i] == v) return i;
    return -1;
}

void relationsArr(char* a, char v) {
    int i = findArr(a, v);
    int sib;
    if (i < 0) { printf("Node %c not found\n", v); return; }

    printf("Node %c:\n", v);
    printf("  Left child  : ");
    if (i * 2 < MAX && a[i * 2]) printf("%c\n", a[i * 2]); else printf("None\n");
    printf("  Right child : ");
    if (i * 2 + 1 < MAX && a[i * 2 + 1]) printf("%c\n", a[i * 2 + 1]); else printf("None\n");

    printf("  Parent      : ");
    if (i == 1) printf("None\n");
    else printf("%c\n", a[i / 2]);

    printf("  Sibling     : ");
    if (i == 1) printf("None\n");
    else {
        sib = (i % 2 == 0) ? i + 1 : i - 1;
        if (sib < MAX && a[sib]) printf("%c\n", a[sib]); else printf("None\n");
    }
}

void freePtr(Node* r) {
    if (!r) return;
    freePtr(r->left);
    freePtr(r->right);
    free(r);
}

/* ===================== 主函数 ===================== */
int main() {
    static char arr[MAX];
    char s[1024];
    int pos, n1, l1, n2, l2, len;
    Node* root;
    char target;

    printf("Input binary tree parenthesis notation: ");
    if (fgets(s, sizeof(s), stdin) == NULL) return 0;

    len = (int)strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';

    memset(arr, 0, sizeof(arr));
    pos = 0;
    parseArr(s, &pos, arr, 1);

    pos = 0;
    root = parsePtr(s, &pos);

    /* ============ 数组实现 ============ */
    printf("\n===== Array Implementation =====\n");
    printf("[1] Tree Print\n");
    printArr(arr, 1, 0);

    printf("\n[2] Tree Info\n");
    n1 = countArr(arr);
    l1 = leafArr(arr);
    printf("  Total nodes    : %d\n", n1);
    printf("  Leaf nodes     : %d\n", l1);
    printf("  Non-leaf nodes : %d\n", n1 - l1);
    printf("  Height         : %d\n", heightArr(arr));
    printf("  Degree         : %d\n", degreeArr(arr));

    printf("\n[3] Shape Check\n");
    printf("  Complete Binary Tree : %s\n", isCompleteArr(arr) ? "Yes" : "No");
    printf("  Full Binary Tree     : %s\n", isFullArr(arr) ? "Yes" : "No");
    printf("  Skewed Binary Tree   : %s\n", isSkewedArr(arr) ? "Yes" : "No");

    printf("\n  Memory used (Array): %d bytes\n", maxIdxArr(arr) + 1);

    /* ============ 指针实现 ============ */
    printf("\n===== Pointer Implementation =====\n");
    printf("[1] Tree Print\n");
    printPtr(root, 0);

    printf("\n[2] Tree Info\n");
    n2 = countPtr(root);
    l2 = leafPtr(root);
    printf("  Total nodes    : %d\n", n2);
    printf("  Leaf nodes     : %d\n", l2);
    printf("  Non-leaf nodes : %d\n", n2 - l2);
    printf("  Height         : %d\n", heightPtr(root));
    printf("  Degree         : %d\n", degreePtr(root));

    printf("\n[3] Shape Check\n");
    printf("  Complete Binary Tree : %s\n", isCompletePtr(root) ? "Yes" : "No");
    printf("  Full Binary Tree     : %s\n", isFullPtr(root) ? "Yes" : "No");
    printf("  Skewed Binary Tree   : %s\n", isSkewedPtr(root) ? "Yes" : "No");

    printf("\n  Memory used (Pointer): %d bytes\n", n2 * (int)sizeof(Node));

    /* ============ 关系查询 ============ */
    printf("\nInput a node value to find relations: ");
    if (scanf(" %c", &target) != 1) return 0;

    printf("\n[Array] Relations\n");
    relationsArr(arr, target);

    printf("\n[Pointer] Relations\n");
    relationsPtr(root, target);

    freePtr(root);
    return 0;
}
