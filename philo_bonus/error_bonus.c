/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:23:04 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 17:04:28 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	close_sems(t_philo *philo)
{
	if (philo->death && philo->death != SEM_FAILED)
		sem_close(philo->death);
	if (philo->forks && philo->forks != SEM_FAILED)
		sem_close(philo->forks);
	if (philo->zen && philo->zen != SEM_FAILED)
		sem_close(philo->zen);
	if (philo->lock && philo->lock != SEM_FAILED)
		sem_close(philo->lock);
	if (philo->print && philo->print != SEM_FAILED)
		sem_close(philo->print);
	sem_unlink(FORKS);
	sem_unlink(DEATH);
	sem_unlink(ZEN);
	sem_unlink(LOCK);
	sem_unlink(PRINT);
}

void	free_and_null(void	**freeable)
{
	if (freeable && *freeable)
	{
		free(*freeable);
		*freeable = NULL;
	}
}

void	handle_waitress(t_philo *philo)
{
	int	i;

	if (philo->food != 0 && philo->waitress)
	{
		i = 0;
		while (i++ < philo->num_philos)
			unlock_sem(philo, philo->zen);
		pthread_join(philo->waitress, NULL);
	}
}

void	exit_threads(t_philo *philo)
{
	if (philo->death_watcher)
		pthread_join(philo->death_watcher, NULL);
	if (philo->vitality)
		pthread_join(philo->vitality, NULL);
	if (philo->waitress)
		handle_waitress(philo);
}

void	error(t_philo *philo, int ret, char *msg)
{
	int	i;

	if (philo->death)
		unlock_sem(philo, philo->death);
	if (philo->forks)
	{
		i = 0;
		while (i++ < philo->num_philos)
			unlock_sem(philo, philo->forks);
	}
	exit_threads(philo);
	if (philo->lock)
	{
		lock_sem(philo, philo->lock);
		free_and_null((void **)&philo->pids);
		free_and_null((void **)&philo->die);
		free_and_null((void **)&philo->fed);
		unlock_sem(philo, philo->lock);
	}
	close_sems(philo);
	if (philo)
		free(philo);
	if (msg)
		printf("%s\n", msg);
	exit(ret);
}
