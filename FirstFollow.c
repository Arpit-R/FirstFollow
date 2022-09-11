#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char **productions;
char **first;
char **follow;
int n;

int isTerminal(char p)
{
	if(!(p>'A' && p<'Z')) return 1;
	return 0;
}

int ProdInd(char c)
{
	int i=0;
	while(productions[i][0] != c)
	{
		i++;
	}
	return i;
}

void AddFirst(int pi,char c)
{
	int i=0;
	while(first[pi][i] != c && i != strlen(first[pi]))
		i++;
	if(i==strlen(first[pi]))
	{
		first[pi][i] = c;
		i++;
		first[pi][i] = ',';
	}
}

void AddFollow(int pi,char c)
{
	int i=0;
	while(follow[pi][i] != c && i != strlen(follow[pi]))
		i++;
	if(i==strlen(follow[pi]))
	{
		follow[pi][i] = c;
		i++;
		follow[pi][i] = ',';
	}
}

void First(int pi)
{
	char f = productions[pi][0];
	char *prod = productions[pi];
	int i,whe=0;
	for (i=2;i<strlen(prod);i++)
	{
		if (prod[i] == '|' || prod[i] == '#')
		{
			AddFirst(pi,'#');
			whe=1;
			continue;
		}
		int skip = 0;
		if(prod[i] == f)
		{
			skip = 1;
		}	
		else if(isTerminal(prod[i]))
		{
			AddFirst(pi,prod[i]);
			skip = 1;
		}
		else
		{
			int ind = ProdInd(prod[i]);
			if(strlen(first[ind]) == 0) First(ind);
			int j=0;
			while(j<strlen(first[ind]))
			{
				if (first[ind][j] != '#')
					AddFirst(pi,first[ind][j]);
				else
					skip = 1;
				j++;
			}
		}
		if (skip)
		{
			while(prod[i] != '|' && i < strlen(prod))
				i++;
		}
	}
	if(whe)
	{
		for (i=2;i<strlen(prod);i++)
		{
			int skip=0;
			if(prod[i]==f && (prod[i-1]== '=' || prod[i-1]== '|'))
			{
				while(prod[i] == f)
				 i++;
				if(prod[i] == '|') continue;
				if(isTerminal(prod[i]))
				{
					AddFirst(pi,prod[i]);
					skip = 1;
				}
				else
				{
					int ind = ProdInd(prod[i]);
					if(strlen(first[ind]) == 0) First(ind);
					int j=0;
					while(j<strlen(first[ind]))
					{
						if (first[ind][j] != '#')
							AddFirst(pi,first[ind][j]);
						else
							skip = 1;
						j++;
					}
				}
			}
			else
			 skip = 1;
			
			if (skip)
			{
				while(prod[i] != '|' && i < strlen(prod))
					i++;
			}
		}
	}
}

void Follow(int pi)
{
	int i;
	char f = productions[pi][0];
	for(i=0;i<n;i++)
	{
		int j;
		char *prod = productions[i];
		for(j=2;j<strlen(prod);j++)
		{
			if(prod[j] == f)
			{
				j++;
				while(prod[j] != '\0' && prod[j] != '|')
				{
					int done = 1;
					if(isTerminal(prod[j]))
					{
						AddFollow(pi,prod[j]);
					}
					else
					{
						int k=0;
						while(k<strlen(first[i]))
						{
							if(first[i][k] != '#')
							{
								AddFollow(pi,first[i][k]);
							}
							else
							{
								done = 0;
							}
							k++;
						}
					}
					if (done) break;
					j++;
				}
				if((prod[j] == '\0' || prod[j] == '|') && prod[0] != f)
				{
					if(strlen(follow[i]) == 0) Follow(i);
					int k=0;
					while(k<strlen(follow[i]))
					{
						AddFollow(pi,follow[i][k]);
						k++;
					}
				}
			}
		}
	}
}

void main()
{
	printf("Enter Number of Productions: ");
	scanf("%d",&n);
	productions = (char**)malloc(n* sizeof(char*));
	first = (char**)malloc(n* sizeof(char*));
	follow = (char**)malloc(n* sizeof(char*));
	int i;
	for(i=0;i<n;i++)
	{
		productions[i] = (char*)calloc(30, sizeof(char));
		first[i] = (char*)calloc(30, sizeof(char));
		follow[i] = (char*)calloc(30, sizeof(char));
	}
	printf("\nEnter Productions (eg. S=aSb|#) use '#' as empty string");
	printf("\nCapital letters will be considered as Variables and other as terminals.");
	printf("\nUse only one letter or symbol for a variable and terminal.");
	
	for(i=0;i<n;i++)
	{
		printf("\n%d. ",i+1);
		scanf("%s",productions[i]);
	}
	
	printf("\n---------------FIRST---------------\n");
	for(i=0;i<n;i++)
	{
		if(strlen(first[i])==0)
		{
			First(i);
		}
		first[i][strlen(first[i]) -1] = '\0';
		printf("\nFIRST(%c) = {%s}",productions[i][0],first[i]);
	}
	
	printf("\n---------------FOLLOW---------------\n");
	AddFollow(0,'$');
	for(i=0;i<n;i++)
	{
		if(strlen(follow[i]) == 0 || i==0)
		{
			Follow(i);
		}
		follow[i][strlen(follow[i]) -1] = '\0';
		printf("\nFOLLOW(%c) = {%s}",productions[i][0],follow[i]);
	}
}
