#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Since the assignment forbids building a tree data structure, 
// we use arrays to store info for nodes A-Z (26 letters).
char parent[26];             // Record the parent of a node
char children[26][26];       // Record the list of children for each node
int child_count[26];         // Record the number of children (degree)
int depth[26];               // Record the depth (level) of each node
int node_exists[26];         // Mark if a node exists

// Stack (required by the assignment to keep track of the current parent path)
char stack[1000];
int stack_size = 0;

// Recursive function to print the tree rotated 90 degrees
void print_tree(int node, char* prefix, int is_last) {
    // If it's the root (depth 0), just print the letter
    if (depth[node] == 0) {
        printf("%c\n", node + 'A');
    } else {
        // Otherwise, print prefix + "+--" + letter
        printf("%s+--%c\n", prefix, node + 'A');
    }
    
    int n = child_count[node];
    for (int i = 0; i < n; i++) {
        int child = children[node][i] - 'A';
        int child_is_last = (i == n - 1); 
        
        char next_prefix[500];
        strcpy(next_prefix, prefix);
        
        // Format according to Image 3:
        // If current node is NOT the last child, add "|  " to prefix for next level.
        // If it IS the last child, add "   " (spaces).
        if (depth[node] > 0) { 
            if (is_last) {
                strcat(next_prefix, "   ");
            } else {
                strcat(next_prefix, "|  ");
            }
        }
        // Recursively print child
        print_tree(child, next_prefix, child_is_last);
    }
}

int main() {
    char input[1000];
    
    // 1. Get input
    if (scanf("%999s", input) != 1) {
        printf("Error: Invalid tree format\n");
        return 0;
    }
    
    int len = strlen(input);
    if (len == 0) {
        printf("Error: Invalid tree format\n");
        return 0;
    }

    // 2. Pre-check parenthesis matching
    int balance = 0;
    for (int i = 0; i < len; i++) {
        if (input[i] == '(') balance++;
        else if (input[i] == ')') balance--;
        if (balance < 0) { 
            printf("Error: Invalid tree format\n");
            return 0;
        }
    }
    if (balance != 0) { 
        printf("Error: Invalid tree format\n");
        return 0;
    }

    // 3. Initialize arrays
    for (int i = 0; i < 26; i++) {
        parent[i] = 0;
        child_count[i] = 0;
        depth[i] = 0;
        node_exists[i] = 0;
    }

    // 4. Sequentially scan the string (Core algorithm)
    char current_node = 0;
    int node_count = 0;

    for (int i = 0; i < len; i++) {
        char ch = input[i];
        
        if (ch >= 'A' && ch <= 'Z') {
            int idx = ch - 'A';
            if (node_exists[idx]) {
                printf("Error: Invalid tree format\n");
                return 0;
            }
            node_exists[idx] = 1;
            node_count++;
            current_node = ch;
            depth[idx] = stack_size; // Current stack size is its depth

            if (stack_size > 0) {
                char p = stack[stack_size - 1];
                int p_idx = p - 'A';
                parent[idx] = p;
                children[p_idx][child_count[p_idx]++] = ch; 
            }
        } 
        else if (ch == '(') {
            if (current_node == 0) {
                printf("Error: Invalid tree format\n");
                return 0;
            }
            stack[stack_size++] = current_node;
        } 
        else if (ch == ')') {
            if (stack_size == 0) {
                printf("Error: Invalid tree format\n");
                return 0;
            }
            stack_size--;
        } 
        else if (ch == ',') {
            // Comma is just a separator
        } 
        else {
            printf("Error: Invalid tree format\n");
            return 0;
        }
    }

    if (stack_size != 0 || node_count == 0) {
        printf("Error: Invalid tree format\n");
        return 0;
    }

    // 5. Calculate statistics
    int leaf_count = 0;      
    int max_degree = 0;      
    int max_depth = 0;       

    for (int i = 0; i < 26; i++) {
        if (node_exists[i]) {
            if (child_count[i] == 0) leaf_count++;
            if (child_count[i] > max_degree) max_degree = child_count[i];
            if (depth[i] > max_depth) max_depth = depth[i];
        }
    }

    int non_leaf_count = node_count - leaf_count;
    int tree_height = max_depth + 1; 

    // 6. Print statistics in English (No more garbled text)
    printf("Total nodes: %d\n", node_count);
    printf("Leaf nodes: %d\n", leaf_count);
    printf("Non-leaf nodes: %d\n", non_leaf_count);
    printf("Tree height: %d\n", tree_height);
    printf("Tree degree: %d\n", max_degree);

    // 7. Process node 'C'
    int c_idx = 'C' - 'A';
    if (node_exists[c_idx]) {
        if (parent[c_idx] != 0) {
            printf("Parent of node 'C': %c\n", parent[c_idx]);
        } else {
            printf("Parent of node 'C': None\n");
        }
        
        if (child_count[c_idx] > 0) {
            printf("Children of node 'C': ");
            for (int i = 0; i < child_count[c_idx]; i++) {
                printf("%c%s", children[c_idx][i], (i == child_count[c_idx] - 1) ? "" : ", ");
            }
            printf("\n");
        } else {
            printf("Children of node 'C': None\n");
        }
    } else {
        printf("Parent of node 'C': None\n");
        printf("Children of node 'C': None\n");
    }

    // 8. Print the tree structure
    printf("\n[ Tree Output ]\n");
    
    for (int i = 0; i < 26; i++) {
        if (node_exists[i] && depth[i] == 0) {
            print_tree(i, "", 1);
            break;
        }
    }

    return 0;
}
