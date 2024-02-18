#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
sem_t semp;
sem_t semp2;
int sayilist[15][15];
int sonuclist[15];
int readcount = 0;
int writeIndex = 0;
int Partition(int sayilist[],int ilk,int son)
{
	int j,t;
	int x = sayilist[son];
	int ct = ilk - 1;
	for(j= ilk; j< son; j++)
	{
		if(sayilist[j] <= x)
		{
			ct = ct + 1;
			t = sayilist[ct];
			sayilist[ct] = sayilist[j];
			sayilist[j] = t; 
		}	
	}
	t = sayilist[ct+1];
	sayilist[ct+1] = sayilist[son];
	sayilist[son] = t; 
	return ct+1;
}
void Quicfonc(int sayilist[],int ilk, int son)
{
	int q;
	if(ilk < son)
	{
		q = Partition(sayilist,ilk,son);
		Quicfonc(sayilist,ilk,q-1);
		Quicfonc(sayilist,(q+1),son);
	}
}
void* threadSort()
{	
	
	int i;
	int c = sayilist[0][1];
	bool test = true;
	while(test)
	{
		sem_wait(&semp2);
		Quicfonc(sonuclist,0,c-1);

		for(i=0; i<c;i++)
		{
			if(i == c-1)
			{
				printf("%d",sonuclist[i]);
			}
			else
			{
				printf("%d ",sonuclist[i]);
			}
			
		}
		test = false;
		sem_post(&semp2);
		
	}
	pthread_exit(NULL);
}
void* ToplamaFonk(void* arg) {
	int ct = *(int*)arg;
	int j;
	int toplam=0;
	int r = sayilist[0][0];
	int c = sayilist[0][1];
  	for(j= 1;j< r+1; j++)
	{
	  toplam+= sayilist[j][ct];
	}
    	sem_wait(&semp); 

	readcount++;	
	if(readcount == 1)
	{
		sem_wait(&semp2); 
	}
	sonuclist[ct] = toplam;
	
	sem_post(&semp);
	
	if(readcount == c)
	{
		sem_post(&semp2);
	}
    	pthread_exit(NULL);
}
int main()
{
	sem_init(&semp, 0, 1);
	sem_init(&semp2, 0, 1);
	int r,c,j,i;
	scanf("%d %d",&r,&c);
	int sayilist1[r+1][c];
	sayilist[0][0] = r;
	sayilist[0][1] = c;
	for(i = 1; i<r+1; i++)
	{
		for(j = 0;j<c;j++ )
		{
			scanf("%d",&sayilist[i][j]);
		}
	}
	pthread_t thlist[c];
	pthread_t threadSort12;
	int konumlar[c];
	for (i = 0; i < c; ++i)
	{
		konumlar[i] = i;
        	pthread_create(&thlist[i], NULL, ToplamaFonk, &konumlar[i]);
    	}
	pthread_create(&threadSort12,NULL,threadSort, NULL);
    	for (i = 0; i < c; ++i) 
    	{
       		pthread_join(thlist[i], NULL);
   	}
  	pthread_join(threadSort12, NULL);
  	sem_destroy(&semp);
  	sem_destroy(&semp2);
}


