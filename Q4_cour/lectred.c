#include "lectred.h"

struct lectred {
	pthread_mutex_t mutex;
	pthread_cond_t lect, red;
	int nb_l;
	int nb_w;
	int nb_w_attente;
};

struct lectred* lectred_init() {
	struct lectred * lectred = malloc(sizeof(struct lectred));
	pthread_mutex_init(&lectred->mutex, NULL);
	pthread_cond_init(&lectred->lect, NULL);
	pthread_cond_init(&lectred->red, NULL);
	lectred->nb_w = 0;
	lectred->nb_l = 0;
	lectred->nb_w_attente = 0;
	return lectred;
}

void begin_read(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	while(lectred->nb_w > 0 || lectred->nb_w_attente > 0) {
		pthread_cond_wait( &lectred->lect, &lectred->mutex);
	}
	lectred->nb_l ++;
	pthread_mutex_unlock(&lectred->mutex);
}

void end_read(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	lectred->nb_l --;
	
	if(lectred->nb_l == 0) {
		pthread_cond_signal( &lectred->red);
	}
	pthread_mutex_unlock(&lectred->mutex);
}

void begin_write(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	lectred->nb_w_attente++; 
	while(lectred->nb_l > 0 || lectred->nb_w > 0) {
		pthread_cond_wait( &lectred->red, &lectred->mutex);
	}
	lectred->nb_w_attente--; 
	lectred->nb_w ++;
	pthread_mutex_unlock(&lectred->mutex);
}

void end_write(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	lectred->nb_w --;
	
	if (lectred->nb_w_attente == 0) {
		pthread_cond_broadcast( &lectred->lect);
	} else {
		pthread_cond_signal( &lectred->red);
	}
	pthread_mutex_unlock(&lectred->mutex);
}

void lectred_destroy(struct lectred* lectred) {
	pthread_mutex_destroy(&lectred->mutex);
	pthread_cond_destroy( &lectred->red);
	pthread_cond_destroy( &lectred->lect);
	free(lectred);
}
