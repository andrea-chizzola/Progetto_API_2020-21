#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {RED, BLACK} color;

struct node{

    int id;
    int score;
    color color;
    struct node *right,*left,*parent;

};

struct node sentry = {.color = BLACK, .parent = NULL, .right = NULL, .left = NULL, .id = -1, .score = -1};
struct node *nil = &sentry;

int Dijkstra(int nodes, int graph[nodes][nodes], int first);
void LeftRotate(struct node **root, struct node *x);
void RightRotate(struct node **root, struct node *y);
void RBInsertFixup(struct node **root, struct node *z);
struct node *RBInsert(struct node **root, int id, int score);
void RBTransplant(struct node **root, struct node *u, struct node *v);
void RBDeleteFixup(struct node **root, struct node *x);
void RBDelete(struct node **root, struct node *z);
struct node *TreeMinimum(struct node *x);
struct node *TreeMaximum(struct node *x);
struct node *TreePredecessor(struct node *x);
void Inorder(struct node *x, int lastId);

int main() {

    struct node *root = nil;
    struct node *last = NULL;
    struct node *tempLast = NULL;
    int i = 0;
    int j = 0;
    int num = 0;
    char *dimensions = malloc(sizeof(char) * (23));
    char *command = malloc(sizeof(char) * 15);
    int nodes = 0;
    int k = 0;
    int maxScore = 0;
    int inserted = 0;
    int id = 0;
    int lastId = 0;

    dimensions = fgets(dimensions, 23, stdin);

    while (dimensions[i] != '\0'){

        if (dimensions[i] >= '0' && dimensions[i] <= '9'){
            num = num * 10 + (dimensions[i] - '0');
            i++;
        } else {

            if (j == 0) {
                nodes = num;
                j++;
            }else
                k = num;

            num = 0;
            i++;
        }

    }

    command = fgets(command, 15, stdin);

    while (command != NULL){

        if(strcmp(command, "AggiungiGrafo\n") == 0){

            int score;
            int rows = 0;
            int column = 0;
            int graph[nodes][nodes];
            char *line = malloc(sizeof(char) * (10 * nodes + nodes + 1));

            while (rows < nodes){

                line = fgets(line, 10 * nodes + nodes + 1, stdin);

                num = 0;
                i = 0;

                while (line[i] != '\0'){

                    if (line[i] >= '0' && line[i] <= '9') {
                        num = num * 10 + (line[i] - '0');
                        i++;
                    } else {
                        graph[rows][column] = num;
                        num = 0;

                        if (column < nodes - 1)
                            column++;
                        else
                            column = 0;

                        i++;
                    }

                }

                rows++;
            }

            free(line);
            score = Dijkstra(nodes, graph, 0);

            if (inserted < k){

                if (inserted == 0 || score >= maxScore) {

                    last = RBInsert(&root, id, score);

                    maxScore = score;
                    lastId = id;

                } else {
                    RBInsert(&root, id, score);
                }

                inserted++;

            } else {

                if (score < maxScore){

                    RBInsert(&root, id, score);
                    RBDelete(&root, last);

                    tempLast = TreePredecessor(last);
                    lastId = tempLast->id;
                    maxScore = tempLast->score;

                    free(last);

                    last = tempLast;
                    tempLast = NULL;

                    inserted++;

                }
            }
            id++;

        } else {

            if (inserted == 0)
                fprintf(stdout, "\n");
            else {
                Inorder(root, lastId);
            }

        }

        command = fgets(command, 15, stdin);

    }

    return 0;

}

int Dijkstra(int n, int graph[n][n], int first) {

    int checked[n];
    int j = 1;
    int min = 0;
    int prox = 0;
    int result = 0;

    memset(checked, 0, sizeof(checked));

    graph[first][0] = 0;
    checked[first] = 1;

    while (j < n - 1){

        for (int i = 1; i < n; i++) {
            if (graph[first][i] > 0 && !checked[i] && (graph[first][i] < min || min == 0)) {
                min = graph[first][i];
                prox = i;
            }
        }

        checked[prox] = 1;

        for (int i = 1; i < n; i++) {
            if (!checked[i]){
                if (graph[prox][i] != 0 && (min + graph[prox][i] < graph[first][i] || graph[first][i] == 0)) {
                    graph[first][i] = min + graph[prox][i];
                }
            }
        }

        min = 0;
        j++;

    }

    for (int i = 0; i < n; i++) {
        result += graph[first][i];
    }

    return result;

}

void LeftRotate(struct node **root, struct node *x){

    struct node *y;

    y = x->right;
    x->right = y->left;

    if(x->right != nil)
        (x->right)->parent = x;

    y->parent = x->parent;

    if(x->parent == nil)
        *root = y;
    else if(x == (x->parent)->left)
        (x->parent)->left = y;
    else  (x->parent)->right = y;

    y->left = x;
    x->parent = y;

}
void RightRotate(struct node **root, struct node *y){

    struct node *x;

    x = y->left;
    y->left = x->right;

    if(x->right != nil)
        (x->right)->parent = y;

    x->parent = y->parent;

    if(y->parent == nil)
        *root = x;
    else if(y == (y->parent)->left)
        (y->parent)->left = x;
    else  (y->parent)->right = x;

    x->right = y;
    y->parent = x;

}
void RBInsertFixup(struct node **root, struct node *z){

    struct node *y;

    while(z->parent->color == RED){

        if(z->parent == z->parent->parent->left) {

            y = z->parent->parent->right;
            if ( y->color == RED ) {

                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;

            } else {

                if ( z == z->parent->right ) {
                    z = z->parent;
                    LeftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(root, z->parent->parent);

            }

        }else {

            y = z->parent->parent->left;
            if ( y->color == RED ) {

                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;

            } else {

                if ( z == z->parent->left ) {
                    z = z->parent;
                    RightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(root, z->parent->parent);

            }
        }

    }
    (*root)->color = BLACK;

}
struct node *RBInsert(struct node **root, int id, int score){

    struct node *y,*x;
    struct node *z = (struct node*)malloc(sizeof(struct node));

    z->id = id;
    z->score = score;
    y = nil;
    x = *root;

    while(x != nil) {
        y = x;
        if (z->score < y->score || (z->score == y->score && z->id < y->id))
            x = x->left;
        else x = x->right;
    }
    z->parent = y;

    if(y == nil)
        *root = z;
    else if(z->score < y->score || (z->score == y->score && z->id < y->id))
        y->left = z;
    else y->right = z;

    z->left = nil;
    z->right = nil;
    z->color = RED;
    RBInsertFixup(root,z);

    return z;

}

void RBTransplant(struct node **root, struct node *u, struct node *v){

    if(u->parent == nil)
        *root = v;
    else if(u == u->parent->left)
        u->parent->left = v;
    else u->parent->right = v;

    v->parent = u->parent;

}

void RBDeleteFixup(struct node **root, struct node *x){

    struct node *w;

    while(x != *root && x->color == BLACK){

        if(x == x->parent->left){
            w = x->parent->right;
            if(w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotate(root, x->parent);
                w = x->parent->right;
            }
            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else {
                if(w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(root, x->parent);
                x = *root;
            }
        } else {
            w = x->parent->left;
            if(w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                RightRotate(root, x->parent);
                w = x->parent->left;
            }
            if(w->right->color == BLACK && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            } else {
                if(w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;

}
void RBDelete(struct node **root, struct node *z){

    struct node *y, *x;
    color yOrig;

    y = z;
    yOrig = y->color;

    if(z->left == nil){
        x = z->right;
        RBTransplant(root, z, z->right);
    } else if (z->right == nil){
        x = z->left;
        RBTransplant(root, z, z->left);
    } else {
        y = TreeMinimum(z->right);
        yOrig = y->color;
        x = y->right;
        if(y->parent == z)
            x->parent = y;
        else {
            RBTransplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RBTransplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(yOrig == BLACK){
        RBDeleteFixup(root, x);
    }

}

struct node *TreeMinimum(struct node *x){

    while(x->left != nil){
        x = x->left;
    }
    return x;

}

struct node *TreeMaximum(struct node *x){

    while (x->right != nil){
        x = x->right;
    }
    return x;

}
struct node *TreePredecessor(struct node *x){

    struct node *y;
    if (x->left != nil){
        return TreeMaximum(x->left);
    }
    y = x->parent;
    while (y != nil && x == y->left){
        x = y;
        y = y->parent;
    }
    return y;

}
void Inorder(struct node *x, int lastId){

    if(x != nil) {

        Inorder(x->left, lastId);
        if (x->id != lastId) {
            fprintf(stdout, "%d ", x->id);
        } else {
            fprintf(stdout, "%d\n", x->id);
        }
        Inorder(x->right, lastId);

    }

}


