#include "lectred.h"

struct lectred {
	pthread_mutex_t mutex;
	int nb_l;
	int nb_w;
	struct list * first;
	struct list * last;
};

struct list {
	pthread_cond_t * ma_cond;
	int est_lect;
	struct list * suivant;
};

struct lectred* lectred_init() {
	struct lectred * lectred = malloc(sizeof(struct lectred));
	pthread_mutex_init(&lectred->mutex, NULL);
	lectred->nb_w = 0;
	lectred->nb_l = 0;
	lectred->first = NULL;
	lectred->last = NULL;
	return lectred;
}

pthread_cond_t * add_thread_list(struct lectred* lectred, int bl) {
	struct list * l = malloc(sizeof(struct list));
	l->ma_cond = malloc(sizeof(pthread_cond_t));
	pthread_cond_init(l->ma_cond, NULL);
	l->est_lect = bl;
	l->suivant = NULL;
	
	if(lectred->first == NULL) { //Liste vide
		lectred->first = l;
		lectred->last = l;
	} else {
		lectred->last->suivant = l;
		lectred->last = l;
	}
	
	return l->ma_cond;
}

void remove_thread_list(struct lectred* lectred) {
	pthread_cond_destroy(lectred->first->ma_cond);
	free(lectred->first->ma_cond);
	if(lectred->first == lectred->last) { //1 seul élément dans la liste
		free(lectred->first);
		lectred->first = NULL;
		lectred->last = NULL;
	} else {
		struct list * tmp = lectred->first;
		lectred->first = lectred->first->suivant;
		free(tmp);
	}
}

void begin_read(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	
	pthread_cond_t * ma_cond = add_thread_list(lectred, 1);
	
	while(lectred->first != NULL && lectred->first->ma_cond != ma_cond || lectred->nb_w > 0) {
		pthread_cond_wait( ma_cond, &lectred->mutex);
	}
	
	lectred->nb_l ++;
	remove_thread_list(lectred);
	
	if(lectred->first != NULL && lectred->first->est_lect) {
		pthread_cond_signal(lectred->first->ma_cond);
	}
	
	pthread_mutex_unlock(&lectred->mutex);
}

void end_read(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	lectred->nb_l --;
	
	if(lectred->nb_l == 0 && lectred->first != NULL && !lectred->first->est_lect) {
		pthread_cond_signal( lectred->first->ma_cond);
	}
	
	pthread_mutex_unlock(&lectred->mutex);
}

void begin_write(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	
	pthread_cond_t * ma_cond = add_thread_list(lectred, 0);
	
	while(lectred->first != NULL && lectred->first->ma_cond != ma_cond || lectred->nb_l > 0 || lectred->nb_w > 0) {
		pthread_cond_wait( ma_cond, &lectred->mutex);
	}
	lectred->nb_w ++;
	remove_thread_list(lectred);
	pthread_mutex_unlock(&lectred->mutex);
}

void end_write(struct lectred* lectred) {
	pthread_mutex_lock(&lectred->mutex);
	lectred->nb_w --;
	if(lectred->first != NULL) {
		pthread_cond_signal(lectred->first->ma_cond);
	}
	pthread_mutex_unlock(&lectred->mutex);
}

void lectred_destroy(struct lectred* lectred) {
	pthread_mutex_destroy(&lectred->mutex);
	free(lectred);
}
