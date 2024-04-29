/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:23:56 by aklein            #+#    #+#             */
/*   Updated: 2024/04/30 02:12:33 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	get_fed(t_philo *philo, int get)
{
	static struct timeval	fed;

	if (sem_wait(philo->lock) == -1)
		error(philo, 1, ERR_SEM_WAIT);
	if (get)
	{
		philo->fed = fed;
		if (sem_post(philo->lock) == -1)
			error(philo, 1, ERR_SEM_POST);
		return ;
	}
	if (gettimeofday(&fed, NULL) == -1)
		error(philo, 1, ERR_TIME);
	if (sem_post(philo->lock) == -1)
		error(philo, 1, ERR_SEM_POST);
}

int	verify_existence(t_philo *philo)
{
	int	ms;
	struct timeval	fed;

	get_fed(philo, 1);
	if (sem_wait(philo->lock) == -1)
		error(philo, 1, ERR_SEM_WAIT);
	fed = philo->fed;
	if (sem_post(philo->lock) == -1)
		error(philo, 1, ERR_SEM_POST);
	ms = get_ms(philo, fed);
	if (ms >= philo->to_die)
		return (0);
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

void	sentient_pause(int ms, t_philo *philo)
{
	struct timeval	start;
	struct timeval	current;
	int				elapsed;

	if (gettimeofday(&start, NULL) == -1)
		error(philo, 1, ERR_TIME);
	elapsed = 0;
	while (elapsed < ms)
	{
		if (!verify_existence(philo))
			print_message(DIE, philo);
		usleep(300);
		if (gettimeofday(&current, NULL) == -1)
			error(philo, 1, ERR_TIME);
		elapsed = (current.tv_sec - start.tv_sec) * 1000;
		elapsed += (current.tv_usec - start.tv_usec) / 1000;
	}
}