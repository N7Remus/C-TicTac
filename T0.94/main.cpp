/* 
 * File:   main.cpp
 * Author: remus
 *
 * Created on 2019. március 31., 20:59
 */

#include <cstdlib>
#include <iostream>
#include <ncurses.h>
#include <curses.h>
using namespace std;

class SubNode {
protected:
    int data;
public:
    SubNode* subnode_next;
    SubNode* subnode_prew;

    void _SubNode() {
        data = 0;
    }

    void setData(int i) {
        data = i;
    }

    int getData() {
        return data;
    }
};

class Node {
public:
    int subs_length_pos;
    int subs_length_neg;

    SubNode* subnode_head;
    SubNode* subnode_tail;

    Node* node_next;
    Node* node_prew;

    Node() {
        this->subs_length_pos = 0;
        this->subs_length_neg = 0;
        this->subnode_head = NULL;
        this->subnode_tail = NULL;
    }

    ~Node() {
        std::cout << "NODE DELETED\n";
    }

    void addToHead(int data) {
        SubNode* subnode = new SubNode();
        subnode->setData(abs(rand() % 10)); //rand() % 10; 
        subnode->subnode_next = this->subnode_head;
        this->subnode_head = subnode;
        this->subs_length_pos++;
    }

    void addToTail(int data) {
        SubNode* subnode = new SubNode();
        subnode->setData(abs(0)); //rand() % 10; //data;
        subnode->subnode_prew = this->subnode_tail;
        this->subnode_tail = subnode;
        this->subs_length_neg++;
    }

    //    void print() {
    //        SubNode* head = this->subnode_head;
    //        int i = 1;
    //        while (head) {
    //            std::cout << i << ": " << head->data << std::endl;
    //            head = head->subnode_next;
    //            i++;
    //        }
    //    }
    //
    //    void print_neg() {
    //        SubNode* tail = this->subnode_tail;
    //        int i = 1;
    //        while (tail) {
    //            std::cout << i << ": " << tail->data << std::endl;
    //            tail = tail->subnode_prew;
    //            i++;
    //        }
    //    }



    //    ~Node();
    //    void addToHead(int data);
    //    void addToTail(int data);
    //    void print();
    //    void print_neg();
};

class LinkedList {
public:
    int length_pos;
    int length_neg;
    Node* node_head;
    Node* node_tail;

    LinkedList() {
        this->length_pos = 0;
        this->length_neg = 0;
        this->node_head = NULL;
        this->node_tail = NULL;
    }

    ~LinkedList() {
        std::cout << "LIST DELETED\n";
    }

    void addNewNodes(int x, int y) {
        if (x < 0) {
            do {
                this->addToTail(y);
            } while (this->length_neg <= abs(x));
        } else {
            do {
                this->addToHead(y);
            } while (this->length_pos <= abs(x));

        }
    }

    void addToHead(int y) {
        Node* node = new Node();
        if (y < 0) {
            while (node->subs_length_neg <= abs(y)) {
                node->addToTail(y);
            }
        } else {
            while (node->subs_length_pos <= abs(y)) {
                node->addToHead(y);
            }
        }
        node->node_next = this->node_head;
        this->node_head = node;
        this->length_pos++;
    }

    void addToTail(int y) {
        Node* node = new Node();
        if (y < 0) {
            while (node->subs_length_neg <= abs(y)) {
                node->addToTail(y);
            }
        } else {
            while (node->subs_length_pos <= abs(y)) {
                node->addToHead(y);
            }
        }
        node->node_prew = this->node_tail;
        this->node_tail = node;
        this->length_neg++;
    }


    //    void print() {
    //        Node* head = this->node_head;
    //        int i = 1;
    //        while (head) {
    //            head->print();
    //            head = head->node_next;
    //            i++;
    //        }
    //    }
    //
    //    void print_neg() {
    //        Node* tail = this->node_tail;
    //        int i = 1;
    //        while (tail) {
    //            tail->print_neg();
    //            tail = tail->node_prew;
    //            i++;
    //        }
    //    }

    //    LinkedList();
    //    ~LinkedList();
    //    void addToHead(int data);
    //    void addToTail(int data);
    //    void print();
    //    void print_neg();
};

LinkedList* list = new LinkedList();

int getValue(int x, int y) {
    Node* node;
    SubNode* subnode;
    int i = 0;
    int j = 0;
    list->addNewNodes(x, y);

    if (x < 0) {
        node = list->node_tail;
        while (subnode) {
            if (i == abs(x))
                break;
            node = node->node_prew;
            i++;
        }
    } else {
        node = list->node_head;
        while (subnode) {
            if (i == abs(x))
                break;
            node = node->node_next;
            i++;
        }
    }
    if (y < 0) {
        subnode = node->subnode_tail;
        while (subnode) {
            if (j == abs(y))
                //                return subnode->data;
                break;
            subnode = subnode->subnode_prew;
            j++;
        }
    } else {
        subnode = node->subnode_head;
        while (subnode) {
            if (j == abs(y))
                break;
            subnode = subnode->subnode_next;
            j++;
        }
    }
    return subnode->getData();
    //    return 0;
}

void setValue(int x, int y, int data) {
    Node* node;
    SubNode* subnode;
    int i = 0;
    int j = 0;
    list->addNewNodes(x, y);

    if (x < 0) {
        node = list->node_tail;
        while (subnode) {
            if (i == abs(x))
                break;
            node = node->node_prew;
            i++;
        }
    } else {
        node = list->node_head;
        while (subnode) {
            if (i == abs(x))
                break;
            node = node->node_next;
            i++;
        }
    }
    if (y < 0) {
        subnode = node->subnode_tail;
        while (subnode) {
            if (j == abs(y))
                //                return subnode->data;
                break;
            subnode = subnode->subnode_prew;
            j++;
        }
    } else {
        subnode = node->subnode_head;
        while (subnode) {
            if (j == abs(y))
                break;
            subnode = subnode->subnode_next;
            j++;
        }
    }
    subnode->setData(data);

}

void t() {
    //    if (list->node_head->subnode_head!=NULL){
    //        list->node_head->subnode_head->setData(9);
    //        if (list->node_head->subnode_head->getData()==9)
    //            exit(9);
    //    }else {
    //        exit(0);
    //    }
    Node* node = list->node_head;
    SubNode* sub = node->subnode_head;
    while (sub) {
        sub->setData(1);
        sub = sub->subnode_next;
    }

}

int main(int argc, char const *argv[]) {
    int current_pos[2] = {0, 0};
    bool win = false;
    int vision = 5;
    int c = 0;
    int x = 1;
    int y = 0;
    int v;
    if (0) {
        //        cout << getValue(-5, -2);
        cout << getValue(-5, 0);
        cout << getValue(0, -2);
        cout << getValue(0, 0);
        cout << getValue(-5, 2);
    } else {
        initscr();
        noecho();
        raw();
        keypad(stdscr, TRUE);
        move(0, 0);
        printw("Kilepeshez (ESC): ");
        do {
            refresh();
            //            move(1, 0);
            if (c == 97 || c == 260)
                current_pos[0] = current_pos[0] - 1;
            /*  w  or up */
            if (c == 119 || c == 259)
                current_pos[1] = current_pos[1] + 1;
            /*  d  or right */
            if (c == 100 || c == 261)
                current_pos[0] = current_pos[0] + 1;
            /*  s or down  */
            if (c == 115 || c == 258)
                current_pos[1] = current_pos[1] - 1;
            /* space*/
            if (c == 32) {
                setValue(current_pos[0], current_pos[1], 1);
                cout << "";
                t();
                cout << "";
            }
            x = 1;
            for (int i = current_pos[0] - vision;
                    i < current_pos[0] + vision + 1;
                    i++) {

                y = 0;
                for (int j = current_pos[0] - vision;
                        j < current_pos[0] + vision + 1;
                        j++) {
                    //                    list->addNewNodes(i, j);
                    //                    cout<<list->getData(i,j);


                    v = getValue(i, j);

                    //NCURSES BUG MIATT
                    cout << "";
                    //NCURSES BUG MIATT
                    move(x, y);
                    printw("%d", v);
                    y++;
                }
                x++;

            }
            move(x + vision + 1, 0);
            printw("x: %d ; y: %d ; char: %d", current_pos[0], current_pos[1], c);
            move(vision + 1, vision);

            v = getValue(0, 0);
            //            cout << "";
            move(22, 22);
            printw("data: %d", v);

        } while ((c = getch()) != 27 && !win);
        endwin();
    }
    delete list;
    return 0;
}

