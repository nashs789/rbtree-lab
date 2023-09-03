#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char *data;
    struct node *next;
} Node;

Node *head = NULL;

Node* set_node(void){
    // malloc 앞에 타입 캐스팅은 필요 없음 (= 컴파일러가 해준다.)
    return (Node*) malloc(sizeof(Node));
}

Node* add_first(char* item){
    Node *temp = set_node();
    temp -> data = item;
    temp -> next = head;
    head = temp;

    return temp;
}

int add_after(Node *prev, char *item){
    if(prev == NULL){
        return 0;
    }

    Node *temp = set_node();
    temp -> data = item; 
    temp -> next = prev -> next;
    prev -> next = temp;
}

Node* remove_first(){
    if(head == NULL){
        return NULL;
    } else {
        Node *temp = head;
        head = head -> next;
        return temp;
    }
}

Node* remove_after(Node *prev){
    if(prev == NULL){
        return NULL;
    } else {
        // free는 어디서 해줘야 하지??
        // head를 free 해주면.... temp도??? ===> dangling pointer????
        // 해봤는데 Segmentation Fault 발생
        Node *temp = prev -> next;
        prev -> next = temp -> next;
        return temp; 
    }
}

// 순회하기 (Traverse)
// 특정 값이 있다면 해당 주소 값 반환
Node* find(char *word){
    Node *temp = head;

    while(temp != NULL){
        if(strcmp(temp -> data, word) == 0){
            return temp;
        }

        temp = temp -> next;
    }

    return NULL;
}

Node* get_node(int index){
    Node *temp = head;

    for(int i = 0; i < index && temp != NULL; i++){
        temp = temp -> next;
    }

    return temp;
}

int add(int index, char *item){
    if (index < 0){
        return 0;
    }

    if(index == 0){
        add_first(item);
    }

    Node *temp = get_node(index);
    temp -> data = item;
    temp -> next = NULL;

    Node *prev = get_node(index - 1);
    if(prev != NULL){
        add_after(prev, item);
        return 1;
    }

    return 0;
}

Node *remove1(int index){
    if(index < 0){
        return NULL;
    }

    if(index == 0){
        return remove_first();
    }

    Node *prev = get_node(index - 1);

    if(prev == NULL){
        return NULL;
    } else {
        return remove_after(prev);
    }
}

// Node *remove(char *item){
//     Node *p = head;
//     Node *prev = NULL;

//     while(p != NULL && strcmp(p -> data, item) != 0){
//         prev = p;
//         p = p -> next;
//     }

//     if(p == NULL){
//         return NULL;
//     }

//     if(prev == NULL){
//         return remove_first();
//     } else {
//         return remove_after(prev);
//     }
// }

int main(void){
    head = set_node();
    head -> data = "Tuesday";
    head -> next = NULL;

    Node *q = set_node();
    q -> data = "Thursday";
    q -> next = NULL;
    head -> next = q;

    q = set_node();
    q -> data = "Monday";
    q -> next = head;
    head = q;

    add_after(q, "Sunday");
    // remove_first();
    remove_after(q);

    Node *p = head;
    
    while(p != NULL){
        printf("%s\n", p -> data);
        p = p -> next;
    }

    if(find("Monday") != NULL){
        printf("찾는 문자가 있음\n");
    }

    Node *result = get_node(2);
    printf("%s\n", result->data);
}