#include <stdio.h>
#include <string.h>

#define MAX 100

char str[MAX];

char nodeStack[MAX];
int countStack[MAX];
int top = -1;

int total = 0;
int leaf = 0;
int nonleaf = 0;
int height = 0;
int degree = 0;

char parentC = ' ';
char childC[MAX];
int childCount = 0;


/* 输出树 */
void printTree(int start, int end, int depth)
{
    int i = start;

    while (i < end)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            int j;

            for (j = 0; j < depth; j++)
                printf("   ");

            if (depth > 0)
                printf("+--");

            printf("%c\n", str[i]);

            /* 找这个节点的括号 */
            if (i + 1 < end && str[i + 1] == '(')
            {
                int count = 1;
                int k = i + 2;

                while (count > 0)
                {
                    if (str[k] == '(')
                        count++;

                    if (str[k] == ')')
                        count--;

                    k++;
                }

                printTree(i + 2, k - 1, depth + 1);

                i = k;
            }
            else
            {
                i++;
            }
        }
        else
        {
            i++;
        }
    }
}


int main()
{
    int i;
    char expected = 'A';

    printf("Enter tree: ");
    scanf("%s", str);

    int len = strlen(str);

    /* 检查输入 */
    for (i = 0; i < len; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
        {
            if (str[i] != expected)
            {
                printf("Invalid tree\n");
                return 0;
            }

            expected++;
            total++;
        }
        else if (str[i] != '(' && str[i] != ')' && str[i] != ',')
        {
            printf("Invalid tree\n");
            return 0;
        }
    }

    if (total == 0)
    {
        printf("Invalid tree\n");
        return 0;
    }


    /* 使用两个栈计算树的信息 */
    for (i = 0; i < len; i++)
    {
        char c = str[i];

        /* 节点 */
        if (c >= 'A' && c <= 'Z')
        {
            top++;
            nodeStack[top] = c;
            countStack[top] = 0;

            if (top + 1 > height)
                height = top + 1;

            /* 如果 C 有父节点 */
            if (c == 'C' && top > 0)
                parentC = nodeStack[top - 1];

            /* 如果当前节点的父节点是 C */
            if (top > 0 && nodeStack[top - 1] == 'C')
            {
                childC[childCount] = c;
                childCount++;
            }
        }

        /* 左括号 */
        else if (c == '(')
        {
            /* 不做处理 */
        }

        /* 逗号 */
        else if (c == ',')
        {
            /* 不做处理 */
        }

        /* 右括号 */
        else if (c == ')')
        {
            if (top < 0)
            {
                printf("Invalid tree\n");
                return 0;
            }

            if (countStack[top] == 0)
                leaf++;
            else
                nonleaf++;

            if (countStack[top] > degree)
                degree = countStack[top];

            top--;

            if (top >= 0)
                countStack[top]++;
        }
    }


    /* 只有一个节点的情况 */
    if (total == 1)
    {
        leaf = 1;
        nonleaf = 0;
        height = 1;
        degree = 0;
    }

    /* 检查括号是否正确 */
    if (top != -1)
    {
        printf("Invalid tree\n");
        return 0;
    }


    printf("\nTotal nodes: %d\n", total);
    printf("Leaf nodes: %d\n", leaf);
    printf("Non-leaf nodes: %d\n", nonleaf);
    printf("Height: %d\n", height);
    printf("Degree: %d\n", degree);

    printf("Parent of C: ");

    if (parentC == ' ')
        printf("None\n");
    else
        printf("%c\n", parentC);

    printf("Children of C: ");

    if (childCount == 0)
    {
        printf("None\n");
    }
    else
    {
        for (i = 0; i < childCount; i++)
        {
            printf("%c", childC[i]);

            if (i < childCount - 1)
                printf(", ");
        }

        printf("\n");
    }

    printf("\nTree:\n");

    printTree(0, len, 0);

    return 0;
}
