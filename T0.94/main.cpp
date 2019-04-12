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
public:
    SubNode* subnode_next;
    SubNode* subnode_prew;
    int data;
};

class Node {
public:
    int subs_length_pos;
    int subs_length_neg;

    SubNode* subnode_head;
    SubNode* subnode_tail;

    Node* node_next;
    Node* node_prew;

    //    Node();

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
        subnode->data = data; //rand() % 10; 
        subnode->subnode_next = this->subnode_head;
        this->subnode_head = subnode;
        this->subs_length_pos++;
    }

    void addToTail(int data) {
        SubNode* subnode = new SubNode();
        subnode->data = data; //rand() % 10; //data;
        subnode->subnode_prew = this->subnode_tail;
        this->subnode_tail = subnode;
        this->subs_length_neg++;
    }

    int getData(int y) {
        if (y < 0) {
            SubNode* tail = this->subnode_tail;
            int i = 0;
            while (i != abs(y)) {
                tail = tail->subnode_prew;
                i++;
            }
            return tail->data;
            //                return tail->data; // == abs(y);
        } else {
            SubNode* head = this->subnode_head;
            int i = 0;
            while (i != abs(y)) {
                //                    cout << "node2";

                head = head->subnode_next;
                i++;
            }

            return head->data;

            //                return head->data; // == abs(y);
        }
        //        return 0;
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
        /*                                eddig töltöm fel
                                          negatív<->positive szám
         */

        if (x < 0) {
            do {
                addToTail(y);
            } while (this->length_neg <= abs(x));
        } else {
            do {
                addToHead(y);
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

    int getData(int x, int y) {
        if (x < 0) {
            Node* tail = this->node_tail;
            int i = 0;
            while (i != abs(x)) {
                tail = tail->node_prew;
                i++;
            }
            return tail->getData(y); //== abs(x);
        } else {
            Node* head = this->node_head;
            int i = 0;
            while (i != abs(x)) {
                head = head->node_next;
                i++;
            }
            return head->getData(y); //== abs(x);
        }
        //        return 0;
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

int main(int argc, char const *argv[]) {
    LinkedList* list = new LinkedList();
    int current_pos[2] = {0, 0};
    bool win = false;
    int vision = 2;
    int c = 0;
    int x = 1;
    list->addNewNodes(0, 100);
    cout << "Size: LL : " << list->length_pos << endl;
    cout << "data : " << list->getData(0, 0) << endl;
    cout << "Size SL : " << list->node_head->subs_length_pos << endl;
    SubNode* head = list->node_head->subnode_head;
    int i = 0;
    while (head) {
        cout << head->data;
        head = head->subnode_next;
        i++;
    }
    cout << endl << "i:" << i << endl;
    
    list->addNewNodes(0, 1011);
    cout << "Size: LL : " << list->length_pos << endl;
    cout << "data : " << list->getData(0, 0) << endl;
    cout << "Size SL : " << list->node_head->subs_length_pos << endl;
    head = list->node_head->subnode_head;
    i = 0;
    while (head) {
        cout << head->data;
        head = head->subnode_next;
        i++;
    }
    cout << endl << "i:" << i;
    
    list->addNewNodes(0, -10);
    cout << "Size: LL : " << list->length_neg << endl;
//    cout << "data : " << list->getData(0, 0) << endl;
    cout << "Size SL : " << list->node_head->subs_length_neg << endl;
    head = list->node_head->subnode_tail;
    i = 0;
    while (head) {
        cout << head->data;
        head = head->subnode_prew;
        i++;
    }
    cout << endl << "i:" << i;
    
    //    list->addNewNodes(0, 1);
    //    cout << "Size LL: "<<list->length_pos << endl;


    //    list->addNewNodes(0, 3);
    //    cout << list->getData(0, 2);

    //            initscr();
    //            noecho();
    //            raw();
    //            keypad(stdscr, TRUE);
    //            move(0, 0);
    //            printw("Kilepeshez (ESC): ");
    //            do {
    //                if (c == 97 || c == 260)
    //                    current_pos[0] = current_pos[0] - 1;
    //                /*  w  or up */
    //                if (c == 119 || c == 259)
    //                    current_pos[1] = current_pos[1] + 1;
    //                /*  d  or right */
    //                if (c == 100 || c == 261)
    //                    current_pos[0] = current_pos[0] + 1;
    //                /*  s or down  */
    //                if (c == 115 || c == 258)
    //                    current_pos[1] = current_pos[1] - 1;
    //        
    //                x = 1;
    //                for (int i = current_pos[0] - vision;
    //                        i < current_pos[0] + vision + 1;
    //                        i++) {
    ////                    move(x, 0);
    ////                    refresh();
    //        
    //                    for (int j = current_pos[0] - vision;
    //                            j < current_pos[0] + vision + 1;
    //                            j++) {
    //                        list->addNewNodes(i, j);
    //                        //                    cout<<list->getData(i,j);
    ////                        printw("%d", list->getData(i, j));
    //                        cout << list->getData(i, j);
    //        
    //                    }
    //                    //            cout << "\n";
    //                    x++;
    //        
    //                }
    //                move(vision + 1, vision);
    //            } while ((c = getch()) != 27 && !win);
    //            endwin();
    //
    //        for (int i = 0; i < 100; ++i) {
    //            list->addToHead(i);
    //            list->addToTail(100 - i);
    //        }
    //        list->print();
    //        list->print_neg();
    //        std::cout << "List Length: -" << list->length_neg << std::endl;
    //        std::cout << "List Length: +" << list->length_neg << std::endl;
    delete list;
    return 0;
}

