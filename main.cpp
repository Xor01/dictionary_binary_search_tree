#include<iostream>
#include <string>
#include <fstream>
using namespace std;
struct Node
{
    string word;
    string meaning;
    Node *left,*right;
};

bool EDITED  = false; // to check if user edit something or delete something
bool found = false;
string FILE_NAME; // data's file name store in a global variable
Node *root = NULL;
Node* FindMin(Node* root);
string removeNewLine(string str);
Node *getNewNode(string word, string meaning);
int getNumberOfWords(); // get number of words in the file
void insertNode(string word, string meaning);
void loadFromFile(string fileName);
void in_order(Node *root);
void search(Node *root, string word, bool* found);
void editMeaning(Node *root, string word, string meaning);
void deleteWord(string key);
void loadToFile(std::ofstream& file, Node* root); // to save updated dictionary to the file 
void destroyBST(Node* root); // to de-allocate all nodes memories
void doesWordExist(Node* root, string word, bool* found);

int main()
{
        char choice;
        char ch;
        string word, meaning;
        ofstream file;

        cout << "ENTER DICTIONARY's NAME: ";
        cin >> FILE_NAME;
        loadFromFile(FILE_NAME);
		do
        {
            cout << R"(
                                     _.
                            _.-----'' `\
                __..-----'''            `.
               <            `\.           '\
               :.              `.           `\
                `:.              `.           `-.
                  `:\ N  o         `.            `+.
                    `:. A  s  i      `.  __.===::::;)
    .         r   I   `: c       ___.__>'::::::a:f/'
         C          A   `.  _,===:::=-'-=-"""''
     i      d  t         '-/:::''
                           ''
              y   
)" << '\n';
            cout << "\tYOU ARE USING " << FILE_NAME << " DICTIONARY\n";
            cout <<"Choose an operation:\n";

            cout << "1- Search\n";
            cout << "2- Edit Meaning\n";
            cout << "3- Insert A New Word\n";
            cout << "4- List All Words with meanings\n";
            cout << "5- Delete Word\n";
            cout << "6- Clear Dictionary From Memory\n";
            cout << "7- Update Dictionary File\n";
            cout << "8- Load Another Dictionary File\n";
            cout << "0- Exit\n";
            cout << "Choice: ";
            cin >> choice;
            switch(choice)
            {

                case '1':
                    cout << "Enter word to search for: ";
                    cin >> word;
                    search(root, word, &found);
                    if(!found)
                        cout << "THIS WORD DOES NOT EXIST IN THE DICTIONARY\n";
                    found = false;
                    break;
                case '2':
                    cout << "Enter a word to change its meaning: ";
                    cin >> word;
                    cout << "Enter New Meaning: ";
                    cin.ignore();
                    getline(cin, meaning);
                    doesWordExist(root, word, &found);
                    if (found)
                    {
                        editMeaning(root, word, meaning);
                        found = false;
                    }
                    else
                    {
                        cout << "This Word does not exist\n";
                        cout << "Would you like to add it (Y/N): ";
                        cin >> ch;
                        if (ch == 'Y' || ch == 'y')
                        {
                            insertNode(word, meaning);
                            cout << word << " HAS BEEN ADDED\n";
                        }
                        else
                        {
                            cout << word << " Adding HAS BEEN DISCARDED \n";
                        }
                    }
                    break;
                case '3':
                    cout << "Enter a new word: ";
                    cin >> word;
                    doesWordExist(root, word, &found);
                    
                    cout << "Enter its meaning: ";
                    cin.ignore();
					getline(cin, meaning);
                    if (found)
                    {
                        cout << "THIS WORD EXIST IN THIS DICTIONARY\n";
                        cout << "WOULD YOU LIKE TO CHANGE ITS MEANING INSTEAD (Y/N): ";
                        cin >> ch;
                        if (ch == 'y' || ch == 'Y')
                        {
                            editMeaning(root, word, meaning);
                            EDITED = true;
                        }
                        else
                        {
                            cout << word << " MEANING UPDATE HAS BEEN DISCARDED\n";
                        }
                        found = false;
                    }
                    else
                    {
                        insertNode(word, meaning);
                        cout << word << " HAS BEEN ADDED \n";
                        EDITED = true;
                    }
                    break;
               case '4':
               		cout << "Word\t\t" <<"Meaning\n";
                    in_order(root);
                    break;
                case '5':
                    cout << "Enter word to delete: ";
                    cin >> word;
                    doesWordExist(root, word,&found);
                    if (found)
                    {
                        cout << "are you sure you want to delete " << word << " type y or n: ";
                        cin >> ch;
                        if (ch == 'y' || ch == 'Y')
                            {
                            	EDITED = true;
                                deleteWord(word);
                            }
                            else
                            {
                                cout << word << " DELETED HAS BEEN DISCARDED\n";
                            }
                        found = false;
                    }
                    else
                    {
                        cout << "SORRY, THIS WORD DOES NOT EXIST IN THIS DICTIONARY\n";
                    }
                    break;
                case '6':
                    if (EDITED)
                    {
                        cout << "DICTIONARY HAS BEEN CHANGED, WOULD YOU LIKE TO SAVE: (Y/N): ";
                        cin >> ch;
                        if (ch == 'Y' || ch == 'y')
                        {
                            file.open(FILE_NAME);
                            loadToFile(file, root);
                            file.close();
                            cout << "CHANGES HAVE BEEN SAVED TO YOUR FILE\n";
                            EDITED = false;
                        }
                        else
                        {
                            cout << "CHANGES HAS BEEN DISCARDED\n";
                        }
                        
                    }
                    if (root)
                    {
                        destroyBST(root);
                        root = NULL;
                        cout << "Dictionary Data Has Been Cleared From The Memory\n";
                        cout << "YOU CAN LOAD DICTIONARY FILE FROM THE MENU\n";
                    }
                    else
                    {
                        cout << "THERE IS NO LOADED DICTIONARY TO CLEAR\n";
                    }
                    
                    
                    break;
                case '7':
                    if(!EDITED)
                    {
                        cout << "*DICTIONARY HAS NOT CHANGED, THERE IS NOTING TO UPDATE\n";
                    }
                    else
                    {
                        cout << "YOU ARE ABOUT TO CHANGE DICTIONARY DATA\n";
                        cout << "ARE YOU SURE: (Y/N)\n";
                        cin >> ch;
                        if (ch == 'Y' || ch == 'y')
                        {
                            file.open(FILE_NAME);
                            loadToFile(file, root);
                            file.close();
                            EDITED = false; // reset edited to no 
                            cout << "\n** DICTIONARY HAS BEEN UPDATED **\n";
                        }
                        else
                        {
                            cout << "\n** DICTIONARY UPDATE HAS BEEN DISCARDED **\n";
                        }
                    }           
                    break;
                case '8':
                    destroyBST(root);
                    cout << "Enter Dictionary File Name: ";
                    cin >> FILE_NAME;
                    loadFromFile(FILE_NAME);
                    break;
                case '0':
                    system("cls");
                    if(EDITED)
                    {
                    	cout << "*YOU HAVE CHANGED/ADDED A WORD\nWOULD YUO LIKE TO SAVE THE NEW UPDATE (Y/N)\n";
                    	cin >> ch;
                    	if(ch == 'Y' || ch == 'y')
                    	{
                    		cout << "\n** DICTIONARY HAS BEEN UPDATED **\n";
                    		file.open(FILE_NAME);
                            loadToFile(file, root);
                            file.close();
						}
						else
						{
							cout << "\n** DICTIONARY UPDATE HAS BEEN DISCARDED **\n";
						}
					}
                    cout << "HAVE A NICE DAY...";
                    destroyBST(root);
                    root = NULL;
                    return 0;
                default:
                    cout << "Wrong option, try again\n";
            }
		    system("pause");
            system("cls");
        }while(choice!=0);

    return 0;
}

Node *getNewNode(string word, string meaning)
{
    Node *newNode = new Node;
    newNode->word   = word;
    newNode->meaning = meaning;
    newNode->left  = NULL;
    newNode->right = NULL;
    return newNode;
}

void insertNode(string word, string meaning)
{
       // Create a new Node containing the new element
       Node *newNode = getNewNode(word, meaning);
    
       // Check if the tree is empty
       if(root == NULL)
       {
         root = newNode;
       }
       
       else
       {
          // Pointer to start traversing from root and traverses downward path to search
          // where the new node to be inserted
          Node *cur = root;
     
          // Pointer to store parent node
          Node *parent = NULL;
    
          while (cur != NULL)
          {
           parent = cur;
           if (newNode->word.compare(cur->word) < 0)
               cur = cur->left;
           else
               cur = cur->right;
          }
   
          if (newNode->word.compare(parent->word) < 0)
            parent->left = newNode;
    
          else
            parent ->right = newNode;
       }

}

void loadFromFile(string fileName=FILE_NAME)
{
    ifstream file;
    file.open(fileName);
    if(!file)
    {
    	cout << "SORRY, THIS FILE COULD NOT BE OPENED\n";
        cout << "PLEAS TRY AGAIN FROM THE MENU\n";
    	return;
	}
    // data file should be in word [new line] meaning [new line] ... 
    // it is not sanitized
    while (file.peek() != EOF)
    {
        string word;
        string meaning;
        getline(file, word);
        getline(file, meaning);
        insertNode(word, meaning);
    }
    cout << FILE_NAME << " HAS BEEN LOADED SUCCESSFULLY\n";
}

void in_order(Node *r) // print sorted elements
{
    if(!r)
    return;
    in_order(r->left);
    cout << r->word << ":\t\t[" << r->meaning << "]\n"; // visiting node
    cout << "-------\n";
    in_order(r->right); // traversing right subtree
}

void search(Node *root, string word, bool* found) 
{
    if (!root)
    return;
	if (root->word.compare(word) == 0)
    {
        cout << word << " means: " <<  root->meaning << "\n";
        *found = true;
        return;
    }      
    search(root->left, word, found);
    search(root->right, word, found);
}

void editMeaning(Node *root, string word, string meaning) 
{

    if (root == NULL)
    return;
    if (root->word.compare(word) == 0)
    {
        root->meaning = meaning;
        cout << word << " MEANING HAS BEEN CHANGED.\n";
        EDITED = true;
        return;
    }
    editMeaning(root->left, word, meaning);
    editMeaning(root->right, word, meaning);
}

Node* FindMin(Node* root)
{
    while(root->left != NULL)
        root = root->left;
    return root;
}

void deleteWord(string key)
{
 
    Node *cur = root;
    
    Node *parent = NULL;
    
    while(cur != NULL && cur->word != key)
    {
        parent = cur;
        
        if(key < cur->word )
            cur = cur -> left;
        else
            cur = cur -> right;
    }
    
    if(cur == NULL)
    {
        cout << "Node not found in BST\n";
        return;
    }
    cout << key << " HAS BEEN DELETED\n";
    
    if(cur -> left == NULL && cur -> right == NULL)
    {
        
        if(cur == root)
            root = NULL;
            
      
        else
        {
            if(parent -> left == cur)
                parent -> left = NULL;
            else
                parent -> right = NULL;
        }
 
        delete cur;
    }
    
    // Case 2: node to be deleted has two children
    else if(cur -> left != NULL && cur -> right != NULL)
    {
        
        Node * successor = FindMin(cur->right);
        
        string temp = successor->word;
        
        deleteWord(successor->word);
       
        cur ->word = temp;
    }
    
    // Case 3: node to be deleted has only one child
    else
    {
        Node *child;
        
        if(cur->left != NULL)
            child = cur -> left;
        else
            child = cur -> right;
            
        
        if(cur == root)
        {
            root = child;
        }
        
        else
        {
            if(parent -> left == cur)
                parent -> left = child;
            else
                parent -> right = child;
        }
      
        delete cur;
    }
}

void loadToFile(std::ofstream& file, Node* root)
{

    if(root == NULL) //base condition to stop
    return;
    
    file << root->word << "\n" << root->meaning << "\n"; // output data in nodes to the file
    loadToFile(file, root->left);
    loadToFile(file, root->right); // traversing right subtree
}

void destroyBST(Node* root)
{
	if(!root)
	return;
	destroyBST(root->left);
	destroyBST(root->right);
	delete(root);
}


int getNumberOfWords()
{
    ifstream file;
    file.open(FILE_NAME);
    
    if (!file)
    {
        return 0;
    }
    else
    {
        string line;
        int ctr = 0;
        while (getline(file, line))
        {
            if (line == " ")
            {
                continue;
            }
            ctr++;
        }    
        ctr/=2;
        return ctr;
    }   
}
void doesWordExist(Node* root, string word, bool* found)
{
    if (!root)
    return; 
    else
    {
        if (root->word.compare(word) == 0)
        {
            *found = true;
        }
        doesWordExist(root->left, word, found);
        doesWordExist(root->right, word, found);
    }
}
