#include <pthread.h>

struct station {
	// FILL ME IN
	pthread_mutex_t mutex;
	pthread_cond_t arrival;
	pthread_cond_t leaving;
	int available_seats;
	int capacity;
	int passengers_on_train;
	int passengers_on_station;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);