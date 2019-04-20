#include <iostream>
#include <stdio.h>

using namespace std;

const int SIZE = 1;

typedef struct Node
    {
    string name = "";
    string phone = "";
    Node* next = NULL;
    Node* prev = NULL;

    Node (const string n, const string p)
        {
        name = n;
        phone = p;
        };
    } Node;

class Double_Linked_List
    {
    public:

    Double_Linked_List ();
    Double_Linked_List (const string name, const string phone);
    ~Double_Linked_List ();
    Node* FindNode (const string name);
    Node* AddNodeBack (const string name, const string phone);
    bool DeleteNode (Node* n);
    Node* UpdateNode (Node* n, const string phone);

    private:

    Node* tail_ = NULL;
    Node* head_ = NULL;
    };

Double_Linked_List::Double_Linked_List (const string name, const string phone)
    {
    tail_ = head_ = new Node (name, phone);
    }

Double_Linked_List::Double_Linked_List ()
    {
    }

Double_Linked_List::~Double_Linked_List ()
    {
    while (head_ != NULL)
        {
        if (head_->prev != NULL)
            delete head_->prev;
        head_ = head_->next;
        }
    }

Node* Double_Linked_List::FindNode (const string n)
    {
    Node* temp = head_;

    if (head_ == NULL)
        return NULL;

    while (temp->next != NULL && temp->name != n)
        temp = temp->next;

    if (temp->name == n)
        return temp;
    else
        return NULL;
    }

Node* Double_Linked_List::AddNodeBack (const string name, const string phone)
    {
    Node* temp = new Node (name, phone);

    if (head_ == NULL)
        tail_ = head_ = temp;
    else if (head_->next == NULL)
        {
        tail_ = temp;
        tail_->prev = head_;
        head_->next = tail_;
        }
    else
        {
        tail_->next = temp;
        temp->prev = tail_;
        tail_ = temp;
        }

    return temp;
    }

Node* Double_Linked_List::UpdateNode (Node* n, const string p)
    {
    if (n == NULL)
        return NULL;
    n->phone = p;

    return n;
    }

bool Double_Linked_List::DeleteNode (Node* n)
    {
    if (n == NULL)
        return false;
    if (n->next != NULL && n->prev != NULL)
        {
        n->next->prev = n->prev;
        n->prev->next = n->next;
        }

    if (head_ == n)
        if (n->next != NULL)
            {
            head_ = n->next;
            head_->prev = NULL;
            }
        else
            head_ = NULL;

    if (tail_ == n)
        if (tail_->prev == NULL)
            tail_ = NULL;
        else
            {
            tail_ = n->prev;
            tail_->next = NULL;
            }

    delete n;

    return true;
    }

class Hash_Table
    {
    public:
    Hash_Table ();
    //~Hash_Table ();
    unsigned int CalculateHash (string name);
    Node* AddItem (string name, string phone);
    Node* EditItem (string name, string phone);
    Node* FindItem (string name);
    bool DeleteItem (string name);

    private:

    size_t size_ = SIZE;
    Double_Linked_List* table_ = new Double_Linked_List[size_];
    };

Hash_Table::Hash_Table ()
    {
    }

unsigned int Hash_Table::CalculateHash (string name)
    {
    unsigned int hash = 0;

	for (unsigned int i = 0; i < name.size(); i++)
        {
		hash += (unsigned char)name[i];
		hash -= (hash << 13) | (hash >> 19);
        }

	return hash % size_;
    }

Node* Hash_Table::AddItem (string name, string phone)
    {
    unsigned hash = CalculateHash (name);

    Node* n = table_[hash].FindNode (name);

    if (n != NULL)
        return NULL;
    else
        return table_[hash].AddNodeBack (name, phone);
    }

Node* Hash_Table::EditItem (string name, string phone)
    {
    unsigned hash = CalculateHash (name);

    Node* n = table_[hash].FindNode (name);

    if (n == NULL)
        return NULL;

    return table_[hash].UpdateNode (n, phone);
    }

Node* Hash_Table::FindItem (string name)
    {
    unsigned hash = CalculateHash (name);

    return table_[hash].FindNode (name);
    }

bool Hash_Table::DeleteItem (string name)
    {
    unsigned hash = CalculateHash (name);

    Node* n = table_[hash].FindNode (name);

    return table_[hash].DeleteNode (n);
    }

int main ()
    {
    int N = 0;
    cin >> N;

    Hash_Table phonebook;
    for (int i = 0; i < N; i++)
        {
        char *c;
        char *n;
        scanf ("%s ", c);
        string command (c);
        if (command == "ADD")
            {
            char *p;
            scanf ("%s %s", n, p);
            string name (n);
            string phone (p);
            Node* user = phonebook.AddItem (name, phone);
            if (user == NULL)
            cout << "ERROR\n";
            }
        else if (command == "PRINT")
            {
            scanf ("%s", n);
            string name (n);
            Node* user = phonebook.FindItem (name);
            if (user == NULL)
                cout << "ERROR\n";
            else
                cout << user->name << ' ' << user->phone << '\n';
            }
        else if (command == "DELETE")
            {
            scanf ("%s", n);
            string name (n);
            if (!phonebook.DeleteItem (name))
                cout << "ERROR\n";
            }
        else if (command ==  "EDITPHONE")
            {
            char *p;
            scanf ("%s %s", n, p);
            string name (n);
            string phone (p);
            Node* user = phonebook.EditItem (name, phone);

            if (user == NULL)
                cout << "ERROR\n";
            }
        else
            cout << "ERROR\n";
        }
    return 0;
    }
