#include <iostream>
#include <pqxx/pqxx>
#include <bits/stdc++.h>
#include "SONode.h"

using namespace std;

void print_sortedSlist(std::map<int, SONode *> &sorted_sub);
void print_sortedOlist(std::map<int, SONode *> &sorted_obj);
void set_ptrs(pqxx::result &r, std::map<int, SONode*> &sorted_sub, std::map<int, SONode*> &sorted_obj);
void append_sNode(SONode* node, SONode** s_head);
void append_oNode(SONode* node, SONode** o_head);
void print_sLList(SONode* head);
void print_oLList(SONode* head);
void runQuery(const string &sql, map<int, SONode *> &sorted_sub, map<int, SONode *> &sorted_obj);

void sorted_s_o(const pqxx::result &R, map<int, SONode *> &sorted_sub, map<int, SONode *> &sorted_obj);

using namespace std;
using namespace pqxx;

int main()
{
    std::string sql;
    std::map<int, SONode*> sorted_sub;  //for storing sorted values of subject and corresponding pointers to linked list
    std::map<int, SONode*> sorted_obj;
    sql = "SELECT s_value, o_value FROM public.po511ordered LIMIT 12";
    try {
        runQuery(sql, sorted_sub, sorted_obj);  //the main function which runs query, returns results and constructs the linked lists
    }catch (exception const &e)
    {
        cout << "Database issues" << endl;
        cerr << e.what() << '\n';
        return 1;
    }
    return 0;
}

void runQuery(const string &sql, map<int, SONode *> &sorted_sub, map<int, SONode *> &sorted_obj) {
        // connect to postgresql database
        connection C("dbname = sampleso user = postgres password = 123 \
      hostaddr = 127.0.0.1 port = 5432");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        }
        /* Create a non-transactional object. */
        nontransaction N(C);
        result R( N.exec( sql ));

        cout << "Found " << R.size() << " entries:\n";
        cout << "Subject" << '\t' << "Object" << '\n';

        //create subject and object sorted lists
        sorted_s_o(R, sorted_sub, sorted_obj);
        cout<<"Sorted sub and obj lists before linked lists construction:"<<'\n';
        print_sortedSlist(sorted_sub);
        print_sortedOlist(sorted_obj);
        // call method to construct subject linked list and object linked list
        set_ptrs(R, sorted_sub, sorted_obj);

        cout<<"Sorted sub and obj lists after linked lists construction:"<<'\n';
        print_sortedSlist(sorted_sub);
        print_sortedOlist(sorted_obj);
        // W.commit();
        C.disconnect ();
}

void sorted_s_o(const result &R, map<int, SONode *> &sorted_sub, map<int, SONode *> &sorted_obj) {
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0] << "\t" << c[1] <<'\n';
        int sub_key = c[0].as<int>();
        int obj_key = c[1].as<int>();
        sorted_sub[sub_key] = NULL;
        sorted_obj[obj_key] = NULL;
    }
}

void set_ptrs(pqxx::result &r, std::map<int, SONode*> &sorted_sub, std::map<int, SONode*> &sorted_obj){
    int i = 1;
    for (result::const_iterator c = r.begin(); c != r.end(); ++c){
        int s_val = c[0].as<int>();
        int o_val = c[1].as<int>();
        SONode* s_head = sorted_sub[s_val]; //the head pointer of the subject as stored in sorted_sub map
        SONode* o_head = sorted_obj[o_val];
        SONode* soNode = new SONode(s_val, o_val);  //create a new SO node with the s and o values from current row of result
        soNode->s_after = NULL; //the new node will always be at the last so its after pointers are always NULL
        soNode->o_after = NULL;
        if (s_head == NULL){    //if current head of subject is null
            soNode->s_before = NULL;
            sorted_sub[s_val] = soNode; //if no pointer to head is present for this subject, then assign the present node's address
        } else{
            append_sNode(soNode, &sorted_sub[s_val]); //if head address of subject's linked list is present then append the present node at its end
        }
        if (o_head == NULL){
            soNode->o_before = NULL;
            sorted_obj[o_val] = soNode;
        } else{
            append_oNode(soNode, &sorted_obj[o_val]);
        }
        cout<<"The complete linked list of current subject after processing "<<i<<" rows of result:"<<'\n';
        print_sLList(sorted_sub[s_val]);    //printing out the current linked list of this subject
        cout<<"The complete linked list of current object after processing "<<i<<" rows of result:"<<'\n';
        print_oLList(sorted_obj[o_val]);
        ++i;
    }
}

void append_sNode(SONode* node, SONode** s_head){
    SONode* temp = *s_head;
    while(temp->s_after != NULL){
        temp = temp->s_after;
    }
    node->s_before = temp;
    temp->s_after = node;
}

void append_oNode(SONode* node, SONode** o_head){
    SONode* temp = *o_head;
    while(temp->o_after != NULL){
        temp = temp->o_after;
    }
    node->o_before = temp;
    temp->o_after = node;
}

void print_sLList(SONode* head){
    SONode* temp = head;
    while(temp != NULL){
        cout<<"<<<<<<<<<<<<<<Node Begins>>>>>>>>>>>>>>>>"<<'\n';
        cout <<"s_value = "<< temp -> s << '\n';
        cout <<"o_value = "<< temp -> o << '\n';
        cout <<"s_before = "<< temp -> s_before << '\n';
        cout <<"s_after = "<< temp -> s_after << '\n';
        cout <<"o_before = "<< temp -> o_before << '\n';
        cout <<"o_after = "<< temp -> o_after << '\n';
        cout<<"<<<<<<<<<<<<<<Node Ends>>>>>>>>>>>>>>>>"<<'\n';
        temp = temp->s_after;
    }
}

void print_oLList(SONode* head){
    SONode* temp = head;
    while(temp != NULL){
        cout<<"<<<<<<<<<<<<<<Node Begins>>>>>>>>>>>>>>>>"<<'\n';
        cout <<"s_value = "<< temp -> s << '\n';
        cout <<"o_value = "<< temp -> o << '\n';
        cout <<"s_before = "<< temp -> s_before << '\n';
        cout <<"s_after = "<< temp -> s_after << '\n';
        cout <<"o_before = "<< temp -> o_before << '\n';
        cout <<"o_after = "<< temp -> o_after << '\n';
        cout<<"<<<<<<<<<<<<<<Node Ends>>>>>>>>>>>>>>>>"<<'\n';
        temp = temp->o_after;
    }
}

void print_sortedOlist(map<int, SONode *> &sorted_obj) {
    cout << "o_value node_pointer" << '\n';
    for(auto &o: sorted_obj)
    {
        cout << o.first << "  " <<'\t'<<o.second;
        cout << '\n';
    }
}

void print_sortedSlist(map<int, SONode *> &sorted_sub) {
    cout << "s_value node_pointer" << '\n';
    for(auto &s: sorted_sub)
    {
        cout << s.first << "  " <<'\t'<<s.second;
        cout << '\n';
    }
}
