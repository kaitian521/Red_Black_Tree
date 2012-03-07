#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <assert.h>
#include <vector>
using namespace std;

struct RBTNode
{
    int key;
    bool color;
    RBTNode *left, *right, *parent;    //you konow what it means
    RBTNode()
    {
        left = right = NULL;
        parent = NULL;
    }
};
class RBT
{
public:
    RBT(){RED = 0; BLACK = 1; root = NULL;}
    ~RBT(){leave(root);};
    void insert(int key);    //insert a node whose value is key
    RBTNode * maxNode(RBTNode *p);    // to get the maximun value of a tree whose "root" is p
    RBTNode * minNode(RBTNode *p);    //to get the minimun value of a tree whose "root" is p
    RBTNode * getRoot();    //it is a public function that get the private root of the class
    void InOrder(RBTNode *p);    // output the value in assending order~~~
    bool isRBT();   //to check if a tree is a red-black tree according its charactor
    RBTNode *search(RBTNode *p, int key);    // to search a node ,if it is in the tree
    void remove(RBTNode *x);    //remove a node whose address is p
    RBTNode* successNode(RBTNode *p);
    RBTNode* procedureNode(RBTNode *p);
private:
    bool RED;
    bool BLACK;
    RBTNode *root;
    int cmp; // record the black path length
    void leave(RBTNode *p);    //delete a node and its all children
    void left_rotate(RBTNode *p);
    void right_rotate(RBTNode *p);
    void insert_fix_up(RBTNode *p);   //to fix up a tree to maintain its charactor after inserting a node into the tree
    bool adhere(RBTNode *p, bool color);    //to determine if there two consective node whose color are all red
    bool blackHeight(RBTNode *p, int height);  // to check if all the black-height is the same
    bool isPos(RBTNode *p);
    void remove_fix_up(RBTNode *, RBTNode *p); //to fix up a tree to maintain its charactor after deleting a node from the tree
};

RBTNode * RBT::successNode(RBTNode *p)
{
    RBTNode *x = p, *y = NULL;
    if(x->right)return minNode(x->right);
    y = x->parent;
    while(y && x == y->right){x = y; y = y->parent;}
    return y;
}

RBTNode * RBT::procedureNode(RBTNode *p)
{
    RBTNode *x = p, *y = NULL;
    if(x->left)return maxNode(x->left);
    y = x->parent;
    while(y && x == y->left){x = y; y = y->parent;}
    return y;
}

void RBT::leave(RBTNode *p)
{
    if(!p)return;
    leave(p->left);
    leave(p->right);
    delete p;
}
RBTNode* RBT::getRoot()
{
    return root;
}

RBTNode *RBT::search(RBTNode *p, int key)
{
    if(!p)return NULL;
    if(p->key == key)return p;
    if(p->key > key)return search(p->left, key);
    return search(p->right, key);
}

RBTNode* RBT::minNode(RBTNode *p)
{
    RBTNode *x = p;
    while(p)
    {
        x = p;
        p = p->left;
    }
    return x;
}

RBTNode* RBT::maxNode(RBTNode *p)
{
    RBTNode *x = p;
    while(p)
    {
        x = p;
        p = p->right;
    }
    return x;
}

void RBT::InOrder(RBTNode *p)
{
    if(!p)return;
    InOrder(p->left);
    printf("%d  ", p->key);
    InOrder(p->right);
}

void RBT::insert(int key)
{
    RBTNode *p, *x, *y;
    p = new RBTNode();
    p->color = RED;
    p->key = key;

    if(!root){p->color = BLACK; root = p; return;}
    x = root;
    while(x)
    {
        y = x;
        if(x->key > key)x = x->left;
        else x = x->right;
    }
    p->parent = y;
    if(y->key > key)y->left = p;
    else y->right = p;
    insert_fix_up(p);
}
int time;
void RBT::left_rotate(RBTNode *p) // let the node lower to the left, and its right child take its place
{
    //cout<<"begin to left rotate"<<endl;
    RBTNode *rightt = p->right;
    p->right = rightt->left;
    rightt->parent = p->parent;
    if(rightt->left)rightt->left->parent = p;
    if(!p->parent)root = rightt;
    else if(p == p->parent->left)p->parent->left = rightt;
    else p->parent->right = rightt;
    p->parent = rightt;
    rightt->left = p;
    return;
}

void RBT::right_rotate(RBTNode *p)
{
    //cout<<"begin to right rotate"<<endl;
    RBTNode *leftt = p->left;
    p->left = leftt->right;
    leftt->parent = p->parent;
    if(leftt->right)leftt->right->parent = p; // there exites a case that leftt->right is NULL~~~
    if(!p->parent)root = leftt;    //notice the root could be changed
    else if(p == p->parent->left)p->parent->left = leftt;
    else p->parent->right = leftt;
    p->parent = leftt;
    leftt->right = p;
    return;
}

void RBT::insert_fix_up(RBTNode *p)
{
    RBTNode *x, *y, *uncle;
    while(1)
    {
        if(!p->parent){p->color = BLACK; break;}  //case 1: p is now root node
        else if(p->parent->color == BLACK){break;}    //case 2: the color of p's parent is BLACK
        else
        {
            if(p->parent->parent->left == p->parent)uncle = p->parent->parent->right;    // to get its uncle
            else uncle = p->parent->parent->left;
            if(uncle && uncle->color == RED)    //case 3: the color of parent and its uncle is all RED
            {
                p->parent->color = BLACK;
                uncle->color = BLACK;
                p->parent->parent->color = RED;
                p = p->parent->parent;
            }
            else //case 4: the color of p's parent is RED, and the color of uncle is BLACK or uncle is nil
            {
                if(p->parent == p->parent->parent->left)
                {
                    if(p == p->parent->right)    //the left-right case, make it be left-left
                    {
                        p = p->parent;
                        left_rotate(p);
                    }
                    p->parent->color = BLACK;   //change the tree by rotating the node
                    p->parent->parent->color = RED;
                    right_rotate(p->parent->parent);
                    break;
                }
                else
                {
                    if(p == p->parent->left)
                    {
                        p = p->parent;
                        right_rotate(p);
                    }
                    p->parent->color = BLACK;
                    p->parent->parent->color = RED;
                    left_rotate(p->parent->parent);
                    break;
                }
            }
        }
    }
}

bool RBT::adhere(RBTNode *p, bool color)
{
    if(!p)return true;
    if(color == RED && p->color == RED)return false;
    return adhere(p->left, p->color) * adhere(p->right, p->color);
}

bool RBT::isPos(RBTNode *p)
{
    if(!p)return true;
    if(p->left && p->left->key > p->key)return false;
    if(p->right && p->right->key < p->key)return false;
    return isPos(p->left) * isPos(p->right);
}

bool RBT::isRBT()
{
    cmp = -1;
    if(root && root->color == RED)return false;
    if(!adhere(root, BLACK))return false;
    if(!blackHeight(root, 0))return false;
    if(!isPos(root))return false;
    return true;
}

bool RBT::blackHeight(RBTNode *p, int height)
{
    if(!p)
    {
        if(cmp != -1 && cmp != height)return false;
        cmp = height;
        //cout<<"ºÚ¸ß¶ÈÊÇ£º"<<height<<endl;
        return true;
    }    /* to calculate its children black-height   */
    return blackHeight(p->left, height + (p->color == BLACK)) * blackHeight(p->right, height + (p->color == BLACK));
}

void RBT::remove(RBTNode *p)    //delete a node
{
    assert(p);     //we must ensure that p is Not NULL
    RBTNode *x, *y;
    if(!p->left || !p->right)y = p;   //y is a node that to be deleted
    else y = minNode(p->right);

    if(y->left)x = y->left;    // x is a node that can replace the place of node y
    else x = y->right;

    if(x)x->parent = y->parent;    // let x->parent = y->parent
    if(y->parent == NULL)    // y is the root
        root = x;
    else if(y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    if(y != p)p->key = y->key;    // copy the data from y to p
    p = y->parent;    //let p be the parent in case that  x is null~~~ it is very important , you know that???
    time = 0;
    if(root && y->color == BLACK)remove_fix_up(x, p);    // if color y is BLACK, we should do something to let the tree be red-black
    delete y;
}

void RBT::remove_fix_up(RBTNode *x, RBTNode *p)     //after delete a node, we must do something to make the tree be a rbt~~~
{
    RBTNode *lchild, *rchild, *bro;
    while(x != root && (!x || x->color == BLACK))
    {
        if(x)p = x->parent;
        if(x == p->left)bro = p->right;
        else bro = p->left;
        if(x == p->left) // x is its parent's left child
        {
            cout<<"left"<<endl;
            if(bro->color == RED)    //case 1: its bro is RED, let make its new brother be BLACK
            {
                p->color = RED;
                bro->color = BLACK;
                time++;
                left_rotate(p);
            }
            else    //its brother's color is BLACK, wo should do something
            {
                lchild = bro->left;
                rchild = bro->right;
                if((!lchild || lchild->color == BLACK) && (!rchild || rchild->color == BLACK)) // case 2
                {
                    bro->color = RED;
                    x = p;
                }
                else if((bro->left && bro->left->color == RED) && (!bro->right || bro->right->color == BLACK))   //case 3
                {
                    bro->left->color = BLACK;
                    bro->color = RED;
                    time++;
                    right_rotate(bro);
                }
                else if(bro->right->color == RED)    //case 4
                {
                    bro->color = p->color;
                    p->color = BLACK;
                    bro->right->color = BLACK;
                    time++;
                    left_rotate(p);
                    x = root;
                    /* what the fuck!, its where put this sentence in case x is changed by the lase sentence pf this function*/
                }
                else    //go this way aha? what the fucking you have done?
                {
                    assert(0 == 1);
                }
             }
        }
        else    //you know it is iso?
        {
            cout<<"right"<<endl;
            if(bro->color == RED)    //case 1: its bro is RED, let make its new brother be BLACK
            {
                p->color = RED;
                bro->color = BLACK;
                time++;
                right_rotate(p);
            }
            else    //its brother's color is BLACK, wo should do something
            {
                lchild = bro->left;
                rchild = bro->right;
                if((!lchild || lchild->color == BLACK) && (!rchild || rchild->color == BLACK)) // case 2
                {
                    bro->color = RED;
                    x = p;
                }
                else if((bro->right && bro->right->color == RED) && (!bro->left || bro->left->color == BLACK))   //case 3
                {
                    bro->right->color = BLACK;
                    bro->color = RED;
                    time++;
                    left_rotate(bro);
                }
                else if(bro->left->color == RED)    //case 4
                {
                    bro->color = p->color;
                    p->color = BLACK;
                    bro->left->color = BLACK;
                    time++;
                    right_rotate(p);
                    x = root;
                }
                else    //go this wat aha? what the fucking you have done?
                {
                    assert(0 == 1);
                }
             }
        }
    }
    x->color = BLACK;
    assert(time < 4);
}

int main()
{
    RBT rbt;
    freopen("2.txt", "r", stdin);
    int n, i, j, t;
    cin>>n;
    for(i = 0; i <= n; i++)
    {
        cin>>t;
        rbt.insert(t);
    }
    cout<<"insertion is over"<<endl;
    //rbt.InOrder(rbt.getRoot());
    if(rbt.isRBT())printf("\ncongratulations...\n");
    //else printf("\nsorry...\n");
    //rbt.InOrder(rbt.getRoot());
    //rbt.remove(rbt.search(rbt.getRoot(), 0));
    //rbt.remove(rbt.search(rbt.getRoot(), -1));
    //if(rbt.isRBT())cout<<"the new tree"<<" is a rbt"<<endl;
    //else cout<<"the new tree"<<" is not a rbt"<<endl;
    //rbt.InOrder(rbt.getRoot());
    //return 0;

    freopen("3.txt", "r", stdin);
    cin>>n;
    for(i = n / 2 + 0; i >= -n / 2; i--)
    {
        cin>>j;
        rbt.remove(rbt.search(rbt.getRoot(), j));
        if(rbt.isRBT())cout<<j<<" is a rbt"<<endl;
        else {cout<<j<<" is not a rbt"<<endl;break;}
        cout<<flush;
    }
    //rbt.InOrder(rbt.getRoot());
    return 0;
}
