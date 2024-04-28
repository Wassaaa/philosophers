/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:37:32 by aklein            #+#    #+#             */
/*   Updated: 2024/04/26 17:15:26 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	destroy_mutex(pthread_mutex_t *mutex)
{
	if (mutex)
	{
		pthread_mutex_destroy(mutex);
		free(mutex);
	}
}

void	free_all(t_philo *philo)
{
	int	i;

	i = 0;
	destroy_mutex(philo->print_lock);
	destroy_mutex(philo->fork_lock);
	destroy_mutex(philo->halt_lock);
	destroy_mutex(philo->food_lock);
	destroy_mutex(philo->start_lock);
	if (philo->forks)
	{
		while (i < philo->num_philos)
			pthread_mutex_destroy(&philo->forks[i++]);
		free(philo->forks);
	}
	if (philo->threads)
		free(philo->threads);
	if (philo->halt_deliberation)
		free(philo->halt_deliberation);
	if (philo->fork_states)
		free(philo->fork_states);
	if (philo->food_finished)
		free(philo->food_finished);
}
