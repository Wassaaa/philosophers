/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:23:56 by aklein            #+#    #+#             */
/*   Updated: 2024/04/29 08:24:16 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

int	verify_existence(t_philo *philo)
{
	if (get_ms(philo, philo->fed) >= philo->to_die)
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