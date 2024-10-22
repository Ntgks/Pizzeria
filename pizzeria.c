#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "helper.h"
#include <time.h>
#include <math.h>

int seed;

int cook = Ncook;
int fournoi = Noven;
int paketarisma = Npacker;
int dianomeis = Ndeliver;

int esoda = 0;
int special = 0;
int plain = 0;
int apotuxhmenes_paraggelies = 0;
int epituxhmenes_paraggelies = 0;
int megistosXronosEksuphrethshs = 0;
int sumTime = 0;
int megistosXronosKruwmatos = 0;
int sumTimeCold = 0;

int firstCustomerflag = 1;

pthread_mutex_t lock;
pthread_cond_t cond;

pthread_cond_t cond_cook;
pthread_cond_t cond_fournoi;
pthread_cond_t cond_paketarisma;
pthread_cond_t cond_dianomeis;

int customers = 0;

double getElapsedTime(struct timespec start, struct timespec end)
{
	return (end.tv_sec -start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}



void *order(void *x)
{
    double time = 0;
    int id = *(int *)x;
    int rc;
    double time_cold = 0;
    
    
    srand(seed);
    if(firstCustomerflag == 1)
    {
	firstCustomerflag = 0;
    }
    else
    {
    	int perimene = (rand() % Torderhigh) + Torderlow;
    	sleep(perimene);
    }
    
    int numberPizzas = (rand() % 5) + 1;


    struct timespec start,end;
    
    
    int XronosGiaParaggelia = (rand() % Norderhigh) + Norderlow;
    sleep(XronosGiaParaggelia);
    
    

    int counter = 0;
    
    while(numberPizzas > counter)
    {
        int tuxh = rand() % (10 + 1);
        if(tuxh <= Pplain)
        {
            esoda += Cplain;
            plain ++;
        }
        else
        {
            esoda +=Cspecial;
            special++;
        }

        counter ++;
    }

    if((rand() % 10) + 1 == Pfail)
    {
        printf("Η παραγγελία με αριθμό <%d> απέτυχε\n", id);
        apotuxhmenes_paraggelies ++;
        rc = pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
    else
    {
    	epituxhmenes_paraggelies ++;
      	int anamonh = (rand() % Tpaymenthigh) + Tpaymentlow;
      	rc = pthread_mutex_unlock(&lock);
      	time  += anamonh;
        sleep(anamonh);
        
        
        
	printf("Η παραγγελία με αριθμό <%d> καταχωρήθηκε\n", id);
	rc = pthread_mutex_lock(&lock);

	clock_gettime(CLOCK_REALTIME,&start);
        while(cook == 0)
        {		
            rc = pthread_cond_wait(&cond_cook,&lock);
        }
        clock_gettime(CLOCK_REALTIME,&end);
        time += getElapsedTime(start,end);

        cook --;
        rc = pthread_mutex_unlock(&lock);

        sleep(Tprep*numberPizzas);
        time += Tprep*numberPizzas;
        
        rc = pthread_mutex_lock(&lock);
        
	clock_gettime(CLOCK_REALTIME,&start);
        while(fournoi <= numberPizzas)
        {
            rc = pthread_cond_wait(&cond_fournoi,&lock);
        }
        clock_gettime(CLOCK_REALTIME,&end);
        time += getElapsedTime(start,end);
        
        cook ++;
	pthread_cond_signal(&cond_cook);
	
        fournoi -= numberPizzas;

        rc = pthread_mutex_unlock(&lock);

        sleep(Tbake);
        time += Tbake;
        
        rc = pthread_mutex_lock(&lock);
        
        
        
	clock_gettime(CLOCK_REALTIME,&start);
        while(paketarisma == 0)
        {
            rc = pthread_cond_wait(&cond_paketarisma,&lock);
        }
        clock_gettime(CLOCK_REALTIME,&end);
        time += getElapsedTime(start,end);
	
        paketarisma --;
        
        rc = pthread_mutex_unlock(&lock);

        sleep(Tpack * numberPizzas);
        time += Tpack * numberPizzas;
        int xronosparaggelias = (int)time;
        printf("Η παραγγελία με αριθμό <%d> ετοιμάστηκε σε <%d> λεπτά\n",id,xronosparaggelias);
        
        rc = pthread_mutex_lock(&lock);
        fournoi += numberPizzas;
	pthread_cond_signal(&cond_fournoi);
        

	int mphke = 0;
	clock_gettime(CLOCK_REALTIME,&start);
        while(dianomeis == 0)
        {
            if (mphke == 0)
            {
            	mphke = 1;
            	paketarisma ++;
            	pthread_cond_signal(&cond_paketarisma);
            }
            rc = pthread_cond_wait(&cond_dianomeis,&lock);
        }
        clock_gettime(CLOCK_REALTIME,&end);
        time += getElapsedTime(start,end);
        sumTimeCold += getElapsedTime(start,end);
        time_cold +=getElapsedTime(start,end);

	
        dianomeis --;
        if(mphke == 0)
        {
	    paketarisma++;
	    pthread_cond_signal(&cond_paketarisma);
	}
        
        rc = pthread_mutex_unlock(&lock);

        int xronosDelivery = (rand() % Tdelhigh) + Tdellow;
        sleep(xronosDelivery);
        time += xronosDelivery;
        sumTimeCold += xronosDelivery;
        time_cold += xronosDelivery;

	rc = pthread_mutex_lock(&lock);
        
        xronosparaggelias = (int)time;
        
        printf("Η παραγγελία με αριθμό <%d> παραδόθηκε σε <%d> λεπτά\n",id,xronosparaggelias);
        
        sumTime += xronosparaggelias;
        if(xronosparaggelias >= megistosXronosEksuphrethshs)
        {
	 	megistosXronosEksuphrethshs = 	xronosparaggelias;
	}
	
	if (time_cold >= megistosXronosKruwmatos)
	{
		megistosXronosKruwmatos = time_cold;
	}
	
	rc = pthread_mutex_unlock(&lock);
	sleep(xronosDelivery);
	rc = pthread_mutex_lock(&lock);
	
	dianomeis++;
	
        rc = pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
}

int main(int argc, char** argv)
{

    if(argc != 3)
    {
        printf("Usage: ./program <seed>  <customers_num>\n");
        return 1;
    }
    seed = atoi(argv[1]);
    
    int N = atoi(argv[2]);
    
    srand(seed);
    
    int id[N];

    int rc;

    pthread_t threads[N];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond_cook, NULL);
    pthread_cond_init(&cond_fournoi, NULL);
    pthread_cond_init(&cond_paketarisma, NULL);
    pthread_cond_init(&cond_dianomeis, NULL);

    for(int i = 0;i < N; i++)
    {
        id[i] = i + 1;
        rc = pthread_create(&threads[i],NULL,order,&id[i]);
    }
    
    for(int i = 0; i<N; i++)
    {
        pthread_join(threads[i],NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_cook);
    pthread_cond_destroy(&cond_fournoi);
    pthread_cond_destroy(&cond_paketarisma);
    pthread_cond_destroy(&cond_dianomeis);
    
    double mesosXronosEksuphrethshs = (double)sumTime/epituxhmenes_paraggelies;
    double mesosXronosKruwmatos = (double)sumTimeCold/epituxhmenes_paraggelies;
    printf("Συνολικά Έσοδα: %d eurw \n",esoda);
    printf("Απλές πίτσες: %d \n",plain);
    printf("Special πίτσες: %d \n",special);
    printf("Αποτυχημένες Παραγγελίες: %d \n",apotuxhmenes_paraggelies);
    printf("Επιτυχημένες Παραγγελίες: %d \n",epituxhmenes_paraggelies);
    printf("Μέγιστος Χρόνος Εξυπηρέτησης: %d λεπτά\n",megistosXronosEksuphrethshs);
    printf("Μέσος Χρόνος Εξυπηρέτησης: %f λεπτά\n",mesosXronosEksuphrethshs);
    printf("Μέσος Χρόνος Κρυώματος: %f λεπτά\n",mesosXronosKruwmatos);
    printf("Μέγιστος Χρόνος Κρυώματος: %d λεπτά\n",megistosXronosKruwmatos);

    return 0;

}

