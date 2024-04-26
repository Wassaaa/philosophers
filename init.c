/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:25:55 by aklein            #+#    #+#             */
/*   Updated: 2024/04/26 16:32:50 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static int	init_forks(t_philo *philo)
{
	int		i;

	i = 0;
	philo->forks = malloc(philo->num_philos * sizeof(pthread_mutex_t));
	philo->fork_states = malloc (philo->num_philos * sizeof(int));
	if (!philo->forks || !philo->fork_states)
		return (0);
	while (i < philo->num_philos)
	{
		philo->fork_states[i] = 0;
		if (pthread_mutex_init(&philo->forks[i], NULL) != 0)
		{
			while (i--)
				pthread_mutex_destroy(&philo->forks[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	new_mutex(pthread_mutex_t **mutex)
{
	*mutex = malloc(sizeof(pthread_mutex_t));
	if (!*mutex)
		return (0);
	if (pthread_mutex_init(*mutex, NULL) != 0)
	{
		free(*mutex);
		*mutex = NULL;
		return (0);
	}
	return (1);
}

static int	init_locks(t_philo *philo)
{
	if (!new_mutex(&philo->print_lock))
		return (0);
	if (!new_mutex(&philo->fork_lock))
		return (0);
	if (!new_mutex(&philo->halt_lock))
		return (0);
	if (!new_mutex(&philo->food_lock))
		return (0);
	if (!new_mutex(&philo->start_lock))
		return (0);
	return (1);
}

int	init_struct(t_philo *philo)
{
	if (!init_locks(philo))
		return (0);
	if (!init_forks(philo))
		return (0);
	philo->halt_deliberation = malloc(sizeof(int));
	if (!philo->halt_deliberation)
		return (0);
	*philo->halt_deliberation = 0;
	philo->food_finished = malloc(sizeof(int));
	if (!philo->food_finished)
		return (0);
	*philo->food_finished = 0;
	return (1);
}
