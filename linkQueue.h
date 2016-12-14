typedef int ElemType;
typedef struct LNode
{
	ElemType data;
	struct LNode * next;
} qnode;

typedef struct
{
	qnode * front;  //head
	qnode * rear;
} linkQueue;

//不是每个节点都需要 front,rear

void initQueue(linkQueue &Q)
{
	Q.front = new qnode;
	Q.rear=Q.front;
	Q.front->next = NULL;


}

bool queueEmpty(linkQueue Q)
{
	return (Q.front == Q.rear);
}

void getQueueFront(linkQueue Q, ElemType &x)
{
	if (queueEmpty(Q))
		cout << "ERROR,EMPTY!\n";
	else
		x = (Q.front->next)->data;
}

void enterQueue(linkQueue &Q, ElemType x)
{
	qnode * P = new qnode;
	P->data = x;
	P->next = NULL;
	Q.rear->next = P;
	Q.rear = P;
	//cout<<"\n\n <--enter queue success-->\n\n";
}

void outQueue(linkQueue &Q, ElemType &x)
{
    //cout<<"<--this is out queue start-->\n";
	qnode * u;
	if (queueEmpty(Q))
		cout << "ERROR,EMPTY!\n";
	else
	{
	//    cout<<"**\n";
		x = (Q.front->next)->data;
	//	cout<<"&&\n";
		u = Q.front->next;
	//	cout<<"@@\n";
		Q.front->next = u->next;
	//	cout<<"^^\n";
		delete u;
		if (Q.front->next == NULL)
			Q.rear = Q.front;
	}
	//cout<<"<--out queue success-->\n";
}
