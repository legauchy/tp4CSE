#include "lectred.h"

struct lectred {
	pthread_mutex_t mutex;
	int nb_l;
	int nb_w;
	struct liste * first;
	struct liste * last;
};

struct liste {
	pthread_cond_t * maCond;
	int est_lect;
	struct liste * suivant;
};

struct lectred* lectred_init() {
	struct lectred * lectred = malloc(sizeof(struct lectred));
	pthread_mutex_init(&lectred->mutex, NULL);
	pthread_cond_init(&lectred->lect, NULL);
	pthread_cond_init(&lectred->red, NULL);
	lectred->nb_w = 0;
	lectred->nb_l = 0;
	lectred->first = NULL;
	lectred->last = NULL;
	return lectred;
}

int add_thread_list(struct lectred* lectred, int bl) {
	if(lectred->first == NULL) { //Liste vide
		struct list * l = malloc(sizeof(struct list));
		l->est_lect = bl;
		l->suivant = NULL;
		lectred->last = l;
	} else {
		struct list * l = malloc(sizeof(struct list));
		l->i = lectred->;
	}
}

void begin_read(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	while(lectred->nb_w > 0) {
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
	while(lectred->nb_l > 0 || lectred->nb_w > 0) {
		pthread_cond_wait( &lectred->red, &lectred->mutex);
	}
	lectred->nb_w ++;
	pthread_mutex_unlock(&lectred->mutex);
}

void end_write(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	lectred->nb_w --;
	pthread_cond_broadcast( &lectred->lect);
	pthread_cond_signal( &lectred->red);
	pthread_mutex_unlock(&lectred->mutex);
}

void lectred_destroy(struct lectred* lectred) {
	pthread_mutex_destroy(&lectred->mutex);
	pthread_cond_destroy( &lectred->red);
	pthread_cond_destroy( &lectred->lect);
	free(lectred);
}
