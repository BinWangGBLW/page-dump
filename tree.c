#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>


struct bin_tree {
                  int data;
                  struct bin_tree * right, * left;
                  int flag;
};

typedef struct bin_tree node;

void insert(node ** tree, int val)
{
    node *temp = NULL;
    if(!(*tree))
    {
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        temp->data = val;
        *tree = temp;
        return;
    }

    if(val < (*tree)->data)
    {
        insert(&(*tree)->left, val);
    }
    else if(val > (*tree)->data)
    {
        insert(&(*tree)->right, val);
    }

}

void print_preorder(node * tree)
{
    if (tree)
    {
        printf("%d\t",tree->data);
        print_preorder(tree->left);
        print_preorder(tree->right);
    }

}

void print_inorder(node * tree)
{
    if (tree)
    {
        print_inorder(tree->left);
        printf("%d\t",tree->data);
        print_inorder(tree->right);
    }
}

void print_postorder(node * tree)
{
    if (tree)
    {
        print_postorder(tree->left);
        print_postorder(tree->right);
        printf("%d\t",tree->data);
    }
}

void deltree(node * tree)
{
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

node* search(node ** tree, int val)
{
    if(!(*tree))
    {
        return NULL;
    }

    if(val < (*tree)->data)
    {
        search(&((*tree)->left), val);
    }
    else if(val > (*tree)->data)
    {
        search(&((*tree)->right), val);
    }
    else if(val == (*tree)->data)
    {
        return *tree;
    }
}

void main()
{
    node *root;
    node *tmp;
    int i;

    root = NULL;
    /* Inserting nodes into tree */
    for (i=0; i<1024; i++)
       insert(&root, i);
        
             
    //insert(&root, 4);
    //insert(&root, 15);
    //insert(&root, 6);
    //insert(&root, 12);
    //insert(&root, 17);
    //insert(&root, 2);

    /* Printing nodes of tree */
    printf("Pre Order Display\n");
    print_preorder(root);
    printf("\n");

    printf("In Order Display\n");
    print_inorder(root);
    printf("\n");

    printf("Post Order Display\n");
    print_postorder(root);
    printf("\n");

    /* Search node into tree */
    while (1)
    {  
       for(i=0; i<1024; i++)
       {
            tmp = search(&root, i);
            if (!tmp)
            {
                perror("Bad value\n");
                exit(2);
            }
        }
    }

    /* Deleting all nodes of tree */
    deltree(root);
}
