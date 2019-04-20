#include <iostream>
#include <algorithm>

using namespace std;

unsigned int HashRs (const string str);

typedef struct Node
    {
    unsigned int key = 0;
    unsigned int height = 0;
    Node* left = NULL;
    Node* right = NULL;
    string name = "";
    unsigned int value = 0;
    unsigned int order = 0;

    Node (const string str, const unsigned int num, const unsigned int i)
        {
        key = HashRs (str);;
        left = NULL;
        right = NULL;
        height = 1;
        name = str;
        value = num;
        order = i;
        };
    } Node;

typedef struct restable
    {
    string* name = new string;
    int value = 0;
    int order = 0;
    } restable;

int Height (Node* p);
int BalanceFactor (Node* p);
void RenewHeight (Node* p);
Node* RotateRight (Node* p);
Node* RotateLeft (Node* p);
Node* Balance (Node* p);
Node* InsertNode (Node* p, const string name, const unsigned int value, const unsigned int key, const unsigned int i);
Node* InsertNodeByName (Node* p, const string name, const unsigned int value, const unsigned int i);
Node* FindMinNode (Node* p);
Node* RemoveMin (Node* p);
bool CompileArray (Node* p, restable** tab);
int Compare (const void* x, const void* y);
Node* UpdateNode (Node* p, string name, const unsigned int value, const unsigned int key, bool* ex);
Node* UpdateNodeByName (Node* base, string name, const unsigned int value, const unsigned int i);

unsigned int HashRs (const string str)
    {
	unsigned int b = 378551;
	unsigned int a = 63689;
	unsigned int hash = 0;

	for (unsigned int i = 0; i < str.size(); i++)
        {
		hash = hash*a + (unsigned char)str[i];
		a *= b;
        }

	return hash;
    }

int Height (Node* p)
    {
	return (p != NULL) ? p->height : 0;
    }

int BalanceFactor (Node* p)
    {
	return Height (p->right) - Height(p->left);
    }

void RenewHeight (Node* p)
    {
	int hl = Height (p->left);
	int hr = Height (p->right);

	p->height = ((hl > hr) ? hl : hr) + 1;
    }

Node* RotateRight (Node* p)
    {
	Node* temp = p->left;
	p->left = temp->right;
	temp->right = p;
	RenewHeight (p);
	RenewHeight (temp);

	return temp;
    }

Node* RotateLeft (Node* p)
    {
	Node* temp = p->right;
	p->right = temp->left;
	temp->left = p;
	RenewHeight (p);
	RenewHeight (temp);

	return temp;
    }

Node* Balance (Node* p)
    {
	RenewHeight (p);

	if (BalanceFactor (p) == 2)
        {
		if (BalanceFactor (p->right) < 0)
			p->right = RotateRight (p->right);
		return RotateLeft (p);
        }
	if (BalanceFactor (p) == -2)
        {
		if (BalanceFactor (p->left) > 0)
			p->left = RotateLeft (p->left);
		return RotateRight (p);
        }

	return p;
    }

Node* InsertNode (Node* p, const string name, const unsigned int value, const unsigned int key, const unsigned int i)
    {
    if (p != NULL)
        {
        if (key >= p->key)
            p->right = InsertNode (p->right, name, value, key, i);
        else if (key < p->key)
            p->left = InsertNode (p->left, name, value, key, i);
        }
    else
        return new Node (name, value, i);

    return Balance (p);
    }

Node* InsertNodeByName (Node* p, const string name, const unsigned int value, const unsigned int i)
    {
    unsigned int key = HashRs (name);
    return InsertNode (p, name, value, key, i);
    }

Node* FindMinNode (Node* p)
    {
	return (p->left != NULL) ? FindMinNode (p->left) : p;
    }

Node* RemoveMin (Node* p)
    {
	if (p->left == NULL)
		return p->right;
	p->left = RemoveMin (p->left);

	return Balance (p);
    }

bool CompileArray (Node* p, restable** tab)
    {
    if (p == NULL)
        return true;

    *((*tab)->name) = p->name;
    (*tab)->value = p->value;
    (*tab)->order = p->order;
    (*tab)++;
    CompileArray (p->right, tab);
    CompileArray (p->left, tab);

    return true;
    }

int Compare (const void* x, const void* y)
    {
    int res = 0;

    if ((res = -(*(restable*)x).value + (*(restable*)y).value) == 0)
        return ((restable*)x)->order - ((restable*)y)->order;
    else
        return res;
    }

Node* UpdateNode (Node* p, string name, const unsigned int value, const unsigned int key, bool* ex)
    {
	if (p == NULL)
        return NULL;
	if (key < p->key)
		p->left = UpdateNode (p->left, name, value, key, ex);
	if (key > p->key)
		p->right = UpdateNode (p->right, name, value, key, ex);
	else if (name != p->name)
        p->right = UpdateNode (p->right, name, value, key, ex);
    else if (value > p->value)
        {
		Node* leftN = p->left;
		Node* rightN = p->right;
		delete p;
		if (rightN == NULL)
            return leftN;
		Node* rightmin = FindMinNode (rightN);
		rightmin->right = RemoveMin (rightN);
		rightmin->left = leftN;
		return Balance (rightmin);
        }
    else
        *ex = true;

	return Balance (p);
    }

 Node* UpdateNodeByName (Node* base, string name, const unsigned int value, const unsigned int i)
    {
    unsigned int key = HashRs (name);
    bool* ex = new bool;
    *ex = false;
    base = UpdateNode (base, name, value, key, ex);
    if (*ex == false)
        {
        delete ex;

        return InsertNodeByName (base, name, value, i);
        }
    delete ex;

    return base;
    }

int main ()
    {
    unsigned int num_participants = 0;
    unsigned int num_tryings = 0;
    cin >> num_participants >> num_tryings;

    string name = "";
    unsigned int value = 0;
    cin >> name >> value;
    Node* p = new Node (name, value, 0);

    for (unsigned int i = 1; i < num_tryings; i++)
        {
        cin >> name >> value;
        p = UpdateNodeByName (p, name, value, i);
        }

    restable* tab = new restable[num_participants];
    restable* cop = tab;

    CompileArray (p, &tab);

    qsort (cop, num_participants, sizeof (restable), Compare);

    for (unsigned int i = 0; i < num_participants; i++)
        cout << *(cop[i].name) << '\n';

    return 0;
    }



















