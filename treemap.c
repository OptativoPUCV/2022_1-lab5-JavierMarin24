#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * arbol = (TreeMap *) malloc (sizeof(TreeMap));
    if(arbol == NULL) return NULL;
    arbol->root  = NULL;
    arbol->current = NULL;
    arbol->lower_than = lower_than;
    return arbol;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
TreeNode *aux = tree->root;
TreeNode *nodo =NULL;
if(searchTreeMap(tree, key)!= NULL)return;
nodo = createTreeNode(key, value);
tree->current=nodo;
while(aux != NULL)
{
     tree->current->parent=aux;
     if(tree->lower_than(key, aux->pair->key)==1)
     {
         aux= aux->left;
     }
     else if(tree->lower_than(key, aux->pair->key) != 1)
     {
        aux= aux->right;
     }
}
if(tree->current->parent == NULL) tree->root = tree->current;
else if(tree->lower_than(key, tree->current->parent->pair->key)==1) tree->current->parent->left=nodo;
else tree->current->parent->right = tree->current;
}

TreeNode * minimum(TreeNode * x){
    TreeNode *aux;
    aux=x;
    while(aux != NULL)
    {
        if (aux->left !=NULL)
        {
            aux=aux->left;
        }
        else{
            return aux;
        }
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
if(tree==NULL) return;
if(node->right == NULL && node->left == NULL)
{
    if(tree->lower_than(node->parent->pair->key, node->pair->key) == 1)
    {
        node->parent->right = NULL;
        
    }
    else
    {
        node->parent->left = NULL;
    }

}
else 
{
    if(node->right != NULL && node->left == NULL)
    {
        if(node == node->parent->left)
        {
            node->parent->left = node->right;
            node->right->parent = node->parent;
            free(node);
        }
        else{
            if(node == node->parent->right)
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
                free(node);
            }
           
        }
    }
    else 
    {   
        if(node->right == NULL && node->left != NULL)
        {
            if(node == node->parent->left)
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
                free(node);
            }            
            else
            {
                if(node == node->parent->right)
                {
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
                    free(node);
                }
            }
        }

    }   
}
if(node->left != NULL && node->right != NULL)
{
    TreeNode *aux = NULL;
    aux=node->right;
    aux = minimum(aux);
    node->pair->key = aux->pair->key;
    node->pair->value= aux->pair->value;
    removeNode(tree, aux);
    
}
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *aux;
    aux = tree->root;
    while(aux != NULL)
    {
        if(tree->lower_than(key, aux->pair->key) == 1)
        {   
            aux = aux->left;
        }
       
       else if(tree->lower_than(aux->pair->key , key) == 1)
       
        {
           aux=aux->right;
        }

        else
        {
            tree->current = aux;
            return aux->pair;

        }
       
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
  TreeNode* UB = NULL;
  TreeNode*aux = tree->root;
 
  if(searchTreeMap(tree,key) != NULL) return searchTreeMap(tree,key);
  else
  {
    while(aux != NULL)
    {

      if((UB == NULL || (tree->lower_than(aux->pair->key, UB->pair->key) == 1)) && (tree->lower_than(key, aux->pair->key) == 1))
      {
        UB = aux;
      } 
      if(tree->lower_than(key, aux->pair->key) == 1)
      {
        aux = aux->left;
      }
      else
      {
        aux = aux->right;
      }
    }

  }
  if(UB == NULL) return NULL;
  return UB->pair;
    
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* first;
    first=minimum(tree->root);
    return first->pair;
}

Pair * nextTreeMap(TreeMap * tree) {

    TreeNode* aux;
    TreeNode* comparador;
    if(tree->current->right != NULL)
    { 
        aux = tree->current->right;
        aux = minimum(aux);
        tree->current = aux;
        return aux->pair;
    }
    else
    {
      comparador = tree->current; 
      while(1)
      {
          if(tree->lower_than(tree->current->pair->key, comparador->pair->key) == 1)
          {
            tree->current = comparador;
            return tree->current->pair;
          }
          else
          {
            comparador = comparador->parent;
          }
          if(comparador == NULL)
          {
            return NULL;
          }
      }
    }
    return NULL;
}