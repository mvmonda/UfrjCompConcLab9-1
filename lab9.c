#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

// Dois semáforos para sincronização das threads
sem_t sem1, sem2;
// Contador para o número de threads no "meio" dos prints
int x = 0; 

// Printa bem-vindo e libera threads do meio
void *bemVindo() {
    printf("Seja bem-vindo!\n");
    sem_post(&sem1);

    pthread_exit(NULL);
}

// Espera o sinal da última thread do meio, e printa volte sempre
void *volteSempre() {

    sem_wait(&sem2);
    printf("Volte sempre!\n");

    pthread_exit(NULL);
}

// Espera um sinal, printa, e libera a próxima thread do meio. Quando a última executar(x == 3), libera semáforo do volte-sempre.
void *printaNoMeio(void* texto) {
    char *string = (char *)texto;

    sem_wait(&sem1);

    printf("%s", string);
    
    if(++x == 3) sem_post(&sem2);

    sem_post(&sem1);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t threads[5];

    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);


    pthread_create(&threads[0], NULL, volteSempre, NULL);
    pthread_create(&threads[1], NULL, bemVindo, NULL);
    pthread_create(&threads[2], NULL, printaNoMeio, (void *)"Aceita um copo d'agua?\n");
    pthread_create(&threads[3], NULL, printaNoMeio, (void *)"Sente-se por favor\n");
    pthread_create(&threads[4], NULL, printaNoMeio, (void *)"Fique a vontade.\n");

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }


}