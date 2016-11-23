

struct node
{
  char word[32];
  int count;
  struct node* next;
};

void initHead(struct node* head);

struct node* append(struct node* new_node,struct node* head);

int search(char word[],struct node* head);

void printlist(struct node* head);

void deletelist(struct node* head);
