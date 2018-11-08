#include <stdio.h>
#include <stdlib.h>
#include "Set.h"

typedef struct strNode * Node;

struct strNode
{
	Type data;
	struct strNode *left, *right, *parent;
};

struct strSet
{
	struct strNode *root;	//desde la raíz llegamos a cualquier elemento
	int size;
	CompareFunc compareFunc;
	PrintFunc printFunc;
	DestroyFunc destroyFunc;
};

void addLink(Node parent, Node child, bool isLeftChild)
{
	if(child != NULL)
	{
		if(isLeftChild == 0)
		{
			parent->right = child;
			child->parent = parent;
		}
		else
		{
			parent->left = child;
			child->parent = parent;
		}
	}
}
void destroy(Node node, DestroyFunc destroyFunc)
{
	if(node != NULL)
	{
		destroy(node->left, destroyFunc);
		destroy(node->right, destroyFunc);
		free(node);
	}
}
void print(Node node, PrintFunc printFunc)
{
	if(node == NULL)
		return;

	printf("{");
	printFunc(node->data);
	print(node->left, printFunc);
	print(node->right, printFunc);
	printf("}");

}
void removeBranch(Set s, Node n, bool b)
{
	Node p ,m;
	bool leftChild;
	if((n->left == NULL) && (n->right == NULL))
	{
		if(s->root == n)
			s->root = NULL;
		else if(n->parent->left == n)
		{
			p = n->parent;
			p->left = NULL;
		}
		else if(n->parent->right == n)
		{
			p = n->parent;
			p->right = NULL;
		}
		free(n);
	}
	if((n->left != NULL) && (n->right == NULL))
	{
		if(s->root == n)
			s->root = n->left;
		else if(n->parent->left == n)
		{
			p = n->parent;
			p->left = n->left;
			leftChild = 1;
			addLink(p, n->left, leftChild);
		}
		else if(n->parent->right == n)
		{
			p = n->parent;
			p->right = n->right;
			leftChild = 0;
			addLink(p, n->right, leftChild);
		}
		free(n);
	}
	if((n->right != NULL) && (n->left == NULL))
	{
		if(s->root == n)
			s->root = n->right;
		else if(n->parent->left == n)
		{
			p = n->parent;
			p->left = n->right;
			leftChild = 1;
			addLink(p, n->right, leftChild);
		}
		else if(n->parent->right == n)
		{
			p = n->parent;
			p->right = n->right;
			leftChild = 0;
			addLink(p, n->right, leftChild);
		}
		free(n);
	}
	if((n->right != NULL) && (n->left != NULL))
	{
		if(n->left->right != NULL)
			m = n->left;
		else
			m = n->right;

		n->data = m->data;
		p = m->parent;
		if((p == s->root) || (p == n))
		{
			p->left = m->left;
			leftChild = 1;
			addLink(p, n->left, leftChild);
		}
		else
		{
			p->right = m->left;
			leftChild = 0;
			addLink(p, n->left, leftChild);
		}
		free(m);
	}
}

Set set_create(CompareFunc cf, PrintFunc pf, DestroyFunc df)
{
	Set s  = (Set)malloc(sizeof(struct strSet));
	s->size = 0;
	s->root = NULL;
	s->compareFunc = cf;
	s->destroyFunc = df;
	s->printFunc = pf;

	return (s);
}

int set_size(Set s)
{
	if(s == NULL)
		return (-1);
	return (s->size);
}

bool set_add(Set s, Type t)
{
	Node n = (Node)malloc(sizeof(struct strNode));
	n->data = t;
	n->left = NULL;
	n->right = NULL;
	n->parent = NULL;

	if(s->root == NULL)
	{
		s->root = n;
		return (TRUE);
	}
	else
	{
		Node curr, p;
		curr = s->root;

		int comp = s->compareFunc(t, curr->data);
		while((curr != NULL) && (comp != 0))
		{
			p = curr;
			comp = s->compareFunc(t, curr->data);
			if(comp == 0)
			{
				free(n);
				return (FALSE);
			}
			if(comp < 0)
			{
				if(curr->left == NULL)
				{
					curr->left = n;
					n->parent = p;
					s->size++;
					return (TRUE);
				}
				else
				{
					curr = curr->left;
				}
			}
			if(comp > 0)
			{
				if(curr->right == NULL)
				{
					curr->right = n;
					n->parent = p;
					s->size++;
					return (TRUE);
				}
				else
				{
					curr = curr->right;
				}
			}
		}
		free(n);
		return (FALSE);
	}
}
bool set_contains(Set s, Type t)
{
	Node curr;
	curr = s->root;
	int comp;

	while(curr != NULL)
	{
		comp = s->compareFunc(t, curr->data);

		if(comp == 0)
			return (TRUE);
		if(comp < 0)
			curr = curr->left;
		if(comp > 0)
			curr = curr->right;
	}
	return (FALSE);
}
void set_destroy(Set s)
{
	if(s->destroyFunc != NULL)
		destroy(s->root, s->destroyFunc);
}
bool set_remove(Set s, Type t)
{
	bool leftChild;
	Node curr = s->root;
	int comp = s->compareFunc(t, curr->data);

	if(s->root == NULL)
		return (FALSE);

	while(1)
	{
		comp = s->compareFunc(t, curr->data);
		if(comp == 0)
			break;
		if(comp > 0)
		{
			curr = curr->right;
			leftChild = FALSE;
		}
		if(comp < 0)
		{
			curr = curr->left;
			leftChild = TRUE;
		}
		if(curr == NULL)
			return (FALSE);
	}
	removeBranch(s, curr, leftChild);
	return (TRUE);

}
void set_print(Set s)
{
	print(s->root, s->printFunc);
}

