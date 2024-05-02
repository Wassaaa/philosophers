/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:39:39 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 08:11:55 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	halt_manager(t_philo *philo, int to_halt)
{
	int	status;

	if (!philo->halt_deliberation || !philo->halt_lock)
		return (1);
	if (to_halt)
	{
		lock_mutex(philo, philo->halt_lock);
		*philo->halt_deliberation = 1;
		unlock_mutex(philo, philo->halt_lock);
		status = 1;
	}
	else
	{
		lock_mutex(philo, philo->halt_lock);
		status = *philo->halt_deliberation;
		unlock_mutex(philo, philo->halt_lock);
	}
	return (status);
}

int	verify_existence(t_philo *philo)
{
	if (get_ms(philo->fed) >= philo->to_die)
		return (0);
	return (1);
}

int	get_ms(struct timeval start)
{
	struct timeval	time;
	int				elapsed;

	elapsed = 0;
	gettimeofday(&time, NULL);
	elapsed = (time.tv_sec - start.tv_sec) * 1000;
	elapsed += (time.tv_usec - start.tv_usec) / 1000;
	return (elapsed);
}

int	sentient_pause(int ms, t_philo *philo)
{
	struct timeval	start;
	struct timeval	current;
	int				elapsed;

	gettimeofday(&start, NULL);
	elapsed = 0;
	while (elapsed < ms)
	{
		if (halt_manager(philo, 0) || !verify_existence(philo))
			return (0);
		usleep(300);
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000;
		elapsed += (current.tv_usec - start.tv_usec) / 1000;
	}
	return (1);
}
