#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
	// FILL ME IN
	pthread_mutex_init(&station->mutex, NULL);
	pthread_cond_init(&station->arrival, NULL);
	pthread_cond_init(&station->leaving, NULL);
	station->available_seats = 0;
	station->passengers_on_station = 0;
	station->passengers_on_train = 0;
}

void
station_load_train(struct station *station, int count)
{
	// FILL ME IN
	station->capacity = count;
	// Leave promptly if there is no free seats or no passengers on station
	if (station->capacity == 0 || !station->passengers_on_station)
	{
		return;
	}
	pthread_mutex_lock(&station->mutex);
	station->available_seats = count;
	// wake up all passengers waiting for the train when train arrive
	if (station->capacity > 0 && station->passengers_on_station > 0)
	{
		pthread_cond_broadcast(&station->arrival);
	}
	// while train have free seats and there are passengers on station >> wait for more passengers
	while (((station->passengers_on_train) < (station->capacity)) && (station->passengers_on_station))
	{
		pthread_cond_wait(&station->leaving, &station->mutex);
	}
	pthread_mutex_unlock(&station->mutex);
	// reset available seats and passengers on train for the next train
	station->available_seats = 0;
	station->passengers_on_train = 0;
}

void
station_wait_for_train(struct station *station)
{
	// FILL ME IN
	// Enter critical section
	pthread_mutex_lock(&station->mutex);
	station->passengers_on_station++;
	// while there is no available seats >> wait for arrival of train
	while (station->passengers_on_station && !station->available_seats)
		pthread_cond_wait(&station->arrival, &station->mutex);
	if (station->available_seats > 0)
		station->available_seats--;
	pthread_mutex_unlock(&station->mutex);
}

void
station_on_board(struct station *station)
{
	// FILL ME IN
	// Enter critical section
	pthread_mutex_lock(&station->mutex);
	// passengers onboarding
	station->passengers_on_station--;
	station->passengers_on_train++;
	// signal the train to leave because either the train is full or there is no passengers on station
	if (station->capacity == station->passengers_on_train || !station->passengers_on_station)
	{
		pthread_cond_signal(&station->leaving);
	}
	pthread_mutex_unlock(&station->mutex);
}
