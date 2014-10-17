#include "lectred.h"

struct lectred {
	sem_t mutex_nb_lecteur;
	sem_t mutex_prio_lect;
	int nb_lecteur;
	sem_t henry;
};

struct lectred* lectred_init() {
	struct lectred * lectred = malloc(sizeof(struct lectred));
	sem_init(&lectred->mutex_nb_lecteur, 0, 1);
	sem_init(&lectred->mutex_prio_lect, 0, 1);
	sem_init(&lectred->henry, 0, 1);
	lectred->nb_lecteur = 0;
	return lectred;
}

void begin_read(struct lectred* lectred) {
	sem_wait(&lectred->mutex_nb_lecteur);
	if(lectred->nb_lecteur == 0) {
		sem_wait(&lectred->henry);
	}
	lectred->nb_lecteur++;
	sem_post(&lectred->mutex_nb_lecteur);
}

void end_read(struct lectred* lectred) {
	sem_wait(&lectred->mutex_nb_lecteur);
	lectred->nb_lecteur--;
	if(lectred->nb_lecteur == 0) {
		sem_post(&lectred->henry);
	}
	sem_post(&lectred->mutex_nb_lecteur);
}

void begin_write(struct lectred* lectred) {
	sem_wait(&lectred->mutex_prio_lect);
	sem_wait(&lectred->henry);
}

void end_write(struct lectred* lectred) {
	sem_post(&lectred->henry);
	sem_post(&lectred->mutex_prio_lect);
}

void lectred_destroy(struct lectred* lectred) {
	sem_destroy(&lectred->henry);
	sem_destroy(&lectred->mutex_nb_lecteur);
	free(lectred);
}
