/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:23:56 by aklein            #+#    #+#             */
/*   Updated: 2024/04/30 10:05:30 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

int	verify_existence(t_philo *philo)
{
	int				ms;
	struct timeval	fed;
	int				die;

	lock_sem(philo, philo->lock);
	fed = *philo->fed;
	die = *philo->die;
	unlock_sem(philo, philo->lock);
	if (die > 0)
		return (0);
	ms = get_ms(philo, fed);
	if (ms >= philo->to_die)
	{
		lock_sem(philo, philo->lock);
		(*philo->die)++;
		unlock_sem(philo, philo->lock);
		print_message(DIE, philo);
		usleep(10000);
		unlock_sem(philo, philo->print);
		return (0);
	}
	return (1);
}

int	get_ms(t_philo *philo, struct timeval start)
{
	struct timeval	time;
	int				elapsed;

	elapsed = 0;
	if (gettimeofday(&time, NULL) == -1)
		error(philo, 1, ERR_TIME);
	elapsed = (time.tv_sec - start.tv_sec) * 1000;
	elapsed += (time.tv_usec - start.tv_usec) / 1000;
	return (elapsed);
}

int	sentient_pause(int ms, t_philo *philo)
{
	struct timeval	start;
	struct timeval	current;
	int				elapsed;

	if (gettimeofday(&start, NULL) == -1)
		error(philo, 1, ERR_TIME);
	elapsed = 0;
	while (elapsed <= ms)
	{
		if (!verify_existence(philo))
			return (0);
		usleep(500);
		if (!verify_existence(philo))
			return (0);
		if (gettimeofday(&current, NULL) == -1)
			error(philo, 1, ERR_TIME);
		elapsed = (current.tv_sec - start.tv_sec) * 1000;
		elapsed += (current.tv_usec - start.tv_usec) / 1000;
	}
	return (1);
}

void	lock_sem(t_philo *philo, sem_t *sem)
{
	if (sem_wait(sem) == -1)
		error(philo, 1, ERR_SEM_WAIT);
}

void	unlock_sem(t_philo *philo, sem_t *sem)
{
	if (sem_post(sem) == -1)
		error(philo, 1, ERR_SEM_POST);
}
