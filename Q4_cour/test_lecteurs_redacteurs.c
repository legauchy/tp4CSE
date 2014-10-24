#include "lectred.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    struct lectred *lecteur_redacteur;
    int iterations;
    int donnee;
} donnees_thread_t;


void dodo(int scale) {
    usleep((random()%1000000)*scale);
}


void *lecteur(void *args) {
    donnees_thread_t *d = args;
    int i, valeur;

    srandom((int) pthread_self());

    for (i=0; i < d->iterations; i++) {
        dodo(2);

        begin_read(d->lecteur_redacteur);

        printf("Thread %x : d�but lecture\n", (int) pthread_self());
        valeur = d->donnee;
        dodo(1);
        printf("Thread %x : ", (int) pthread_self());
        if (valeur != d->donnee)
            printf("LECTURE INCOHERENTE !!!\n");
        else
            printf("lecture coh�rente\n");

        end_read(d->lecteur_redacteur);
    }
    pthread_exit(0);
}


void *redacteur(void *args) {
    donnees_thread_t *d = args;
    int i, valeur;

    srandom((int) pthread_self());

    for (i=0; i < d->iterations; i++) {
        dodo(2);

        begin_write(d->lecteur_redacteur);

        printf("Thread %x : d�but r�daction......\n", (int) pthread_self());
        valeur = random();
        d->donnee = valeur;
        dodo(1);
        printf("Thread %x : ", (int) pthread_self());
        if (valeur != d->donnee)
            printf("REDACTION INCOHERENTE !!!\n");
        else
            printf("r�daction coh�rente......\n");

        end_write(d->lecteur_redacteur);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t *threads, *thread_courant;
    donnees_thread_t donnees_thread;
    int i, nb_lecteurs, nb_redacteurs;
    void *resultat;

    if (argc < 4) {
        fprintf(stderr, "Utilisation: %s nb_lecteurs nb_redacteurs "
                        "nb_iterations\n", argv[0]);
        exit(1);
    }

    nb_lecteurs = atoi(argv[1]);
    nb_redacteurs = atoi(argv[2]);
    donnees_thread.iterations = atoi(argv[3]);

    threads = malloc((nb_lecteurs+nb_redacteurs)*sizeof(pthread_t));
    thread_courant = threads;
    donnees_thread.lecteur_redacteur = lectred_init();

    for (i=0; i<nb_lecteurs; i++)
        pthread_create(thread_courant++, NULL, lecteur, &donnees_thread);
    for (i=0; i<nb_redacteurs; i++)
        pthread_create(thread_courant++, NULL, redacteur, &donnees_thread);

    for (i=0; i<nb_lecteurs+nb_redacteurs; i++)
        pthread_join(threads[i], &resultat);
    lectred_destroy(donnees_thread.lecteur_redacteur);
    free(threads);
    return 0;
}
