#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 50


typedef struct package{char name[150]; char description[300]; double weight; int address;int priority;} PACKAGE; // structure for representing a package
typedef struct node_l{PACKAGE info; struct node_l *next;} NODE_LIST; // structure for a node in the priority queue
typedef struct node_graph{int index; int address;} NODE_GRAPH; // structure for a graph node
typedef struct graph{int n; NODE_GRAPH *nodes;int** ms;} GRAPH; // structure for a graph


// Function that adds packages to the beginning of a singly linked list
void add_list(NODE_LIST **phead, PACKAGE *p) {
    NODE_LIST *new_node = (NODE_LIST*)malloc(sizeof(NODE_LIST));
    new_node->info.address=p->address;
    new_node->info.priority=p->priority;
    new_node->info.weight=p->weight;
    strcpy(new_node->info.name,p->name);
    strcpy(new_node->info.description,p->description);
    new_node->next = *phead;
    *phead = new_node;
}

// Function that deletes the node with the specified address from the list
int delete_node(NODE_LIST **head, int address)
{
  if (*head == 0)
    return 0;

  NODE_LIST *p = 0;
  if ((*head)->info.address==address)
  {
    p = (*head);
    (*head) = (*head)->next;
  }
  else
  {
    NODE_LIST *pr = (*head);
    for (p = (*head)->next; p && (p->info.address!=address);p = p->next)
    pr = p;
    if (p == 0 )
      return 0;
    pr->next = p->next;
  }
  free(p);
  return 1;
}


// Function that counts how many nodes are in the list
int numberNodes(NODE_LIST *phead){
    int number=0;
    while(phead){
        number++;
        phead=phead->next;
    }
    return number;
}

// Function that deletes the list and releases the allocated memory
void delete_list(NODE_LIST **phead) {
    while (*phead) {
        NODE_LIST *q = (*phead)->next;
        free(*phead);
        *phead = q;
    }
}

// Function that checks if the list is empty
int isListEmpty(NODE_LIST *head) {
    return (head == NULL);
}

// Function that adds packages to the priority queue so that packages with higher priority are placed first in the queue
void add_pqueue(NODE_LIST **pf, NODE_LIST **pr, PACKAGE *pa) {
    NODE_LIST *p, *new_n = (NODE_LIST *)malloc(sizeof(NODE_LIST));
    new_n->info=*pa;
    new_n->next = 0;
    if (*pf == 0 || (*pf)->info.priority <= pa->priority) {
        new_n->next = *pf;
        *pf = new_n;
    }
    else {
        p = *pf;
        while (p->next && (p->next->info.priority > pa->priority)) {
            p = p->next;
        }
        new_n->next = p->next;
        p->next = new_n;
    }
    if (!new_n->next) {
        *pr = new_n;
    }
}

// Function for initializing the predecessor matrix
void initT(GRAPH *g, int t[][MAX]){
  int i, j;
  for (i = 0; i<g->n; i++)
    for (j = 0; j<g->n; j++)
      if (i == j || g->ms[i][j] == 0)
        t[i][j] = -1;
      else
        t[i][j] = i;
}

// Function that loads a graph from a text file (based on the adjacency matrix) and initializes the graph's weight matrix as well as the predecessor matrix
GRAPH readGraph(FILE* file, int w[MAX][MAX], int t[][MAX]) {
  int i, j; GRAPH gr; int addr;
  fscanf(file, "%d\n", &gr.n);
  gr.nodes = malloc(sizeof(NODE_GRAPH)*gr.n);
  gr.ms = malloc(sizeof(int*)*gr.n);
  for (i = 0; i < gr.n; ++i) {
    fscanf(file,"%d",&addr);
    gr.nodes[gr.nodes[i].index = i].address = addr;
    gr.ms[i] = malloc(sizeof(int)*gr.n);
  }
  for (i = 0; i < gr.n; ++i)
    for (j = 0; j < gr.n; ++j) {
      fscanf(file, "%d", &gr.ms[i][j]);
      w[i][j] = i == j ? 0 : (gr.ms[i][j] ? gr.ms[i][j] : INFINITY);
    }
  initT(&gr, t);
  return gr;
}

// Function that loads packages from a CSV file and stores them in a singly linked list
void readCSV(FILE *f,NODE_LIST **phead){
    char read[1200];
    char* tokens;
    while(fgets(read, sizeof(read),f)){
        PACKAGE *pack = (PACKAGE*)malloc(sizeof(PACKAGE));

        tokens = strtok(read, ",");
        strcpy(pack->name, tokens);

        tokens = strtok(NULL, ",");
        strcpy(pack->description, tokens);

        tokens = strtok(NULL, ",");
        pack->weight = atof(tokens);

        tokens = strtok(NULL, ",");
        pack->address = atoi(tokens);

        tokens = strtok(NULL, ",");
        pack->priority = atoi(tokens);

        add_list(phead, pack);
    }
}

// Function that loads packages into a postal delivery vehicle
char* vehicle_packages(NODE_LIST *pf,NODE_LIST **head,double v_capacity,int number){
    FILE *fp;
    char *fileName = (char *)malloc(20 * sizeof(char));
    sprintf(fileName,"unsuccessful_%d.txt",number);
    double weight=0;
    NODE_LIST *q=pf;
    while(q!=NULL){
        if(weight+q->info.weight<=v_capacity){
            weight=weight+q->info.weight;
            add_list(head,&q->info);}
        else{
            if(fp=fopen(fileName,"a+")){
                fprintf(fp,"(*)Name:%s,description:%s,weight:%lf,address:%d,priority:%d\n",q->info.name,q->info.description,q->info.weight,q->info.address,q->info.priority);
                fclose(fp);
            }
        }
        q=q->next;
    }
    return fileName;
}

// Function that calculates the percentage of loaded packages
double percentage(NODE_LIST *pf,NODE_LIST *head){
    int n1,n2;
    double x;
    n1=numberNodes(pf);
    n2=numberNodes(head);
    x=(100*n2)/n1;
    return x;
}

// Floyd's algorithm for finding the matrix of shortest distances and the predecessor matrix
void floyd(GRAPH *g, int d[][MAX], int t[][MAX])
{
  int i, j, k;
  for (k = 0; k<g->n; k++)
    for (i = 0; i<g->n; i++)
      for (j = 0; j<g->n; j++)
        if (d[i][j]>d[i][k] + d[k][j])
        {
          d[i][j] = d[i][k] + d[k][j];
          t[i][j] = t[k][j];
        }
}

// Function for printing the path between given nodes
void path(int i, int j, GRAPH *g, int t[][MAX], int dest)
{
  if (i == j)
    printf("%d -> ", g->nodes[i].address);
  else if (t[i][j] == -1)
    printf("No path found.");
  else {
    path(i, t[i][j], g, t, dest);
    printf("%d", g->nodes[j].address);
    dest != j ? printf(" -> ") : printf("");
  }
}

// Function that finds the index of the node with the given address
int indexOfNode(GRAPH g, int address) {
  int i;
  for (i = 0; i < g.n; ++i)
    if (g.nodes[i].address==address) return i;
  fprintf(stderr, "does not exist");
  exit(500);
  return -1;
}

// Function that prints the trajectory, i.e., the delivery route of the postal vehicle
int trajectory(int d[][MAX], int t[][MAX], int start, NODE_LIST **head, GRAPH g) {
    int closestNode;
    int minDistance;
    int startn = start;
    int s=0;
    PACKAGE packages;
    while (!isListEmpty(*head)) {
        closestNode = -1;
        minDistance = INT_MAX;
        NODE_LIST *temp = *head;
        while (temp != NULL) {
            int k = indexOfNode(g, temp->info.address);
            if (d[startn][k] < minDistance) {
                minDistance = d[startn][k];
                closestNode = temp->info.address;
                packages=temp->info;
            }
            temp = temp->next;
        }

        if (closestNode != -1) {
            int cn = indexOfNode(g, closestNode);
            s += d[startn][cn];
            path(startn, cn, &g, t, cn);
            delete_node(head, closestNode);
            startn = cn;
        }
        printf("\n(The vehicle stopped to deliver the package: name:%s,address:%d)\n",packages.name,packages.address);
    }
    s += d[startn][start];
    path(startn, start, &g, t, start);
    printf("\n(The delivery is completed and the vehicle has returned back to the post office)\n");
    return s;
}

int main() {
  FILE *file,*f,*file_r; GRAPH g;
  int start=0,s,num=1;
  char filename[40],filecsv[40],c,str[5],undelivered[500],*file_undelivered;
  double mass,x;
  NODE_LIST *ph=NULL,*fr=NULL,*r=NULL,*h=NULL;
  int i, j, t[MAX][MAX];
  int w[MAX][MAX];
  do{
      printf("================================================================================\n");
      printf("Choose an option by entering the corresponding character: \nDisplay delivery simulation [S], Exit [E]\n");
      printf("================================================================================\n");
      scanf(" %c",&c);
      if(c =='S'){
          printf("Enter the filename containing the city's adjacency matrix\n");
          scanf("\n%s",filename);
          printf("Enter the filename containing the packages data\n");
          scanf("%s",filecsv);
          printf("Enter the vehicle’s load capacity, i.e., the maximum weight the vehicle can carry\n");
          scanf("%lf",&mass);
          if (file = fopen(filename, "r")) {
            g = readGraph(file, w, t);
            fclose(file);}
          if(f =fopen(filecsv,"r")){
            readCSV(f,&ph);
          while(ph!=NULL){
            add_pqueue(&fr,&r,&ph->info);
            ph=ph->next;
          }
          delete_list(&ph);
          file_undelivered=vehicle_packages(fr,&h,mass,num);
          x=percentage(fr,h);
          printf("\nLoaded packages:\n");
          printf("(name,description,weight,address,priority)\n");
          NODE_LIST *tempp=h;
          while(tempp!=NULL){
               printf("(*) %s, %s, %lf, %d, %d\n",tempp->info.name,tempp->info.description,tempp->info.weight,tempp->info.address,tempp->info.priority);
               tempp=tempp->next;
          }
          fclose(f);
          }
          floyd(&g,w,t);
          printf("\nThe route the vehicle takes during delivery:\n");
          printf("------------------------------------------------\n");
          s=trajectory(w,t,start,&h,g);
          printf("\nDistance traveled:%dkm",s);
          printf("\nPercentage of delivered packages:%lf%%\n",x);
          printf("Would you like to see which packages didn’t fit in the vehicle?[YES]\n");
          scanf(" %s",str);
          if((strcmp(str,"YES"))==0){
            file_r=fopen(file_undelivered,"r");
            if(file_r!=NULL){
                while(fgets(undelivered,500,file_r)!=NULL)
                  printf("%s",undelivered);
            }
            fclose(file_r);
          }
          num=num+1;
          delete_list(&fr);
      }

  }while(c!='E');
      return 0;
}
