#include "lectred.h"

struct lectred {
	sem_t mutex_nb_lecteur_en_cour;
	int nb_lecteur_en_cour;
	
	sem_t mutex_nb_writ_en_attente;
	int nb_writ_en_attente;
	sem_t sem_prio_writ;
	
	sem_t sem_principal;
};

struct lectred* lectred_init() {
	struct lectred * lectred = malloc(sizeof(struct lectred));
	sem_init(&lectred->mutex_nb_lecteur_en_cour, 0, 1);
	lectred->nb_lecteur_en_cour = 0;
	
	sem_init(&lectred->sem_prio_writ, 0, 1);
	sem_init(&lectred->mutex_nb_writ_en_attente, 0, 1);
	lectred->nb_writ_en_attente = 0;
	
	sem_init(&lectred->sem_principal, 0, 1);
	return lectred;
}

void begin_read(struct lectred* lectred) {
	sem_wait(&lectred->sem_prio_writ); //// NAN ! //
	sem_post(&lectred->sem_prio_writ);
	sem_wait(&lectred->mutex_nb_lecteur_en_cour);
	if(lectred->nb_lecteur_en_cour == 0) {
		sem_wait(&lectred->sem_principal);
	}
	lectred->nb_lecteur_en_cour++;
	sem_post(&lectred->mutex_nb_lecteur_en_cour);
}

void end_read(struct lectred* lectred) {
	sem_wait(&lectred->mutex_nb_lecteur_en_cour);
	lectred->nb_lecteur_en_cour--;
	if(lectred->nb_lecteur_en_cour == 0) {
		sem_post(&lectred->sem_principal);
	}
	sem_post(&lectred->mutex_nb_lecteur_en_cour);
}

void begin_write(struct lectred* lectred) {
	sem_wait(&lectred->mutex_nb_writ_en_attente);
	if(lectred->nb_writ_en_attente == 0) {
		sem_wait(&lectred->sem_prio_writ);
	}
	lectred->nb_writ_en_attente++;
	sem_post(&lectred->mutex_nb_writ_en_attente);
	sem_wait(&lectred->sem_principal);
}

void end_write(struct lectred* lectred) {
	sem_wait(&lectred->mutex_nb_writ_en_attente);
	lectred->nb_writ_en_attente--;
	if(lectred->nb_writ_en_attente == 0) {
		sem_post(&lectred->sem_prio_writ);
	}
	sem_post(&lectred->mutex_nb_writ_en_attente);
	sem_post(&lectred->sem_principal);
}

void lectred_destroy(struct lectred* lectred) {
	sem_destroy(&lectred->mutex_nb_lecteur_en_cour);
	
	sem_destroy(&lectred->sem_prio_writ);
	sem_destroy(&lectred->mutex_nb_writ_en_attente);
	
	sem_destroy(&lectred->sem_principal);

	free(lectred);
}
