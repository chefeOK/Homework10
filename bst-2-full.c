#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
/*이진 정렬 트리의 작업은 주로 다음을 포함합니다.
1. 찾기: 키가 있는지 재귀적으로 찾습니다.
2. 삽입: 키가 원래 트리에 없으면 키를 삽입하고 true를 반환하고, 그렇지 않으면 false를 반환합니다.
3. 구성: 루프의 삽입 작업.
4. 삭제: (1) 리프 노드: 원본 트리에 영향을 주지 않고 직접 삭제합니다.
(2) 왼쪽 또는 오른쪽 하위 트리만 있는 노드: 노드가 삭제된 후 전체 왼쪽 하위 트리 또는 오른쪽 하위 트리를 노드가 삭제된 위치로 이동하고 부모의 비즈니스를 상속합니다.
(3) 왼쪽과 오른쪽 서브트리가 모두 있는 노드: 삭제할 노드 p의 직전 전임자 s를 찾아 노드 p를 s로 바꾸고 노드 s를 삭제합니다.*/


typedef struct node {
	int key;
	struct node* left;
	struct node* right;
} Node;

/* 스택용 */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();
void push(Node* aNode);

/* 큐용 */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

Node* deQueue();
void enQueue(Node* aNode);


int initializeBST(Node** h);

/* 구현해야 하는 함수 */
void recursiveInorder(Node* ptr);	  /* 재귀 중위 순회 */
void iterativeInorder(Node* ptr);    /* 반복적인 중위 순회 */
void levelOrder(Node* ptr);	          /* 레벨 순서 순회 */
int insert(Node* head, int key);      /* 트리에 노드 삽입 */
int deleteNode(Node* head, int key);  /* 키에 대한 노드 삭제 */
int freeBST(Node* head); /* 트리에 할당된 모든 메모리 해제 */

/* 필요한 경우 정의된 함수를 추가할 수 있습니다. */


void printStack();



int main()
{
	char command;
	int key;
	Node* head = NULL;

	do {
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

	/* 트리가 비어 있지 않으면 트리에서 할당된 모든 노드를 제거합니다*/
	if (*h != NULL)
		freeBST(*h);

	/* 헤드 노드 생성 */
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}



void recursiveInorder(Node* ptr)
{
	if (ptr) {
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/*
 교과서: p 224
 */
void iterativeInorder(Node* node)
{
	for (;;)
	{
		for (; node; node = node->left)
			push(node);
		node = pop();

		if (!node) break;
		printf(" [%d] ", node->key);

		node = node->right;
	}
}

/*
 교과서: p 225
 */
void levelOrder(Node* ptr)
{
	// 정수 앞 = 뒤 = -1;

	if (!ptr) return; /* 빈 트리 */

	enQueue(ptr);

	for (;;)
	{
		ptr = deQueue();
		if (ptr) {
			printf(" [%d] ", ptr->key);

			if (ptr->left)
				enQueue(ptr->left);
			if (ptr->right)
				enQueue(ptr->right);
		}
		else
			break;

	}

}


int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/* head->left가 루트 */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while (ptr != NULL) {

		/* 키에 대한 노드가 있으면 그냥 반환 */
		if (ptr->key == key) return 1;

		/* 자식 노드로 이동해야 합니다.
		* parentNode를 사용하여 부모를 계속 추적 */
		parentNode = ptr;

		/* 키 비교, 현재 노드의 키가 입력 키보다 큰 경우
		* 그러면 새 노드가 오른쪽 하위 트리에 삽입되어야 합니다.
		* 그렇지 않으면 왼쪽 하위 트리.
		*/
		if (ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* 새 노드를 부모 노드에 연결 */
	if (parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key)
{
	if (head == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	if (head->left == NULL) {
		printf("\n Nothing to delete!!\n");
		return -1;
	}

	/* head->left가 루트 */
	Node* root = head->left;



	Node* parent = NULL;
	Node* ptr = root;

	while ((ptr != NULL) && (ptr->key != key)) {
		if (ptr->key != key) {

			parent = ptr;	/* 부모를 저장 */

			if (ptr->key > key)
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}
	}

	/* 키에 대한 노드가 없습니다 */
	if (ptr == NULL)
	{
		printf("No node for key [%d]\n ", key);
		return -1;
	}

	/*
	* 경우 1: 제거해야 하는 노드는 리프 노드입니다.
	*/
	if (ptr->left == NULL && ptr->right == NULL)
	{
		if (parent != NULL) { /* 부모 존재, 부모의 좌우 링크 조정 */
			if (parent->left == ptr)
				parent->left = NULL;
			else
				parent->right = NULL;
		}
		else {
			/* 부모는 null이며 삭제할 노드가 루트임을 의미합니다. */
			head->left = NULL;

		}

		free(ptr);
		return 1;
	}

	/*
	case 2: 삭제할 노드에 자식이 하나 있는 경우
	*/
	if ((ptr->left == NULL || ptr->right == NULL))
	{
		Node* child;
		if (ptr->left != NULL)
			child = ptr->left;
		else
			child = ptr->right;

		if (parent != NULL)
		{
			if (parent->left == ptr)
				parent->left = child;
			else
				parent->right = child;
		}
		else {
			/* 부모는 null이며 삭제할 노드가 루트임을 의미합니다.
			* 이고 루트에는 자식이 하나 있으므로 자식이 루트여야 합니다.
			*/
			root = child;
		}

		free(ptr);
		return 1;
	}

	/*
	* 경우 3: 노드(ptr)가 두 개의 자식을 가짐
	*
	* 우리는 ptr의 왼쪽 서브트리에서 가장 큰 자손 노드를 찾아야 합니다.
	* 또는 ptr의 오른쪽 하위 트리에서 가장 작은 자손.
	*
	* ptr의 오른쪽 하위 트리에서 가장 작은 자손을 찾습니다.
	*
	*/

	Node* candidate;
	parent = ptr;


	candidate = ptr->right;

	/* 가장 작은 노드는 ptr의 오른쪽 하위 트리에서 가장 왼쪽에 있는 노드입니다. */
	while (candidate->left != NULL)
	{
		parent = candidate;
		candidate = candidate->left;
	}

	/* 후보 노드는 삭제해야 하는 오른쪽 노드입니다.
	* 후보자의 왼쪽이 null임을 유의하십시오.
	*/
	if (parent->right == candidate)
		parent->right = candidate->right;
	else
		parent->left = candidate->right;

	/* ptr을 제거하는 대신 ptr의 키만 변경합니다.
	* 후보 노드의 키로 후보 노드 제거
	*/

	ptr->key = candidate->key;

	free(candidate);
	return 1;
}


void freeNode(Node* ptr)
{
	if (ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head)
{

	if (head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}



Node* pop()
{
	if (top < 0) return NULL;
	return stack[top--];
}

void push(Node* aNode)
{
	stack[++top] = aNode;
}


void printStack()
{
	int i = 0;
	printf("--- stack ---\n");
	while (i <= top)
	{
		printf("stack[%d] = %d\n", i, stack[i]->key);
	}
}


Node* deQueue()
{
	if (front == rear) {
		// printf("\n....Now Queue is empty!!\n" );
		return NULL;
	}

	front = (front + 1) % MAX_QUEUE_SIZE;
	return queue[front];

}

void enQueue(Node* aNode)
{
	rear = (rear + 1) % MAX_QUEUE_SIZE;
	if (front == rear) {
		// printf("\n....Now Queue is full!!\n");
		return;
	}

	queue[rear] = aNode;
}
