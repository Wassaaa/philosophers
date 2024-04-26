/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:29:21 by aklein            #+#    #+#             */
/*   Updated: 2024/04/26 17:08:17 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static pthread_t	launch_philo(t_philo philo)
{
	pthread_t		thread;
	t_philo			*p;

	p = malloc(sizeof(t_philo));
	if (!p)
		return (0);
	*p = philo;
	gettimeofday(&p->start, NULL);
	p->fed = p->start;
	if (pthread_create(&thread, NULL, &existential_cycle, p) != 0)
		return (0);
	return (thread);
}

void	start_threads(t_philo *philo, int *i)
{
	pthread_mutex_lock(philo->start_lock);
	while (*i < philo->num_philos)
	{
		philo->id = *i;
		philo->threads[*i] = launch_philo(*philo);
		if (philo->threads[*i] == 0)
		{
			halt_manager(philo, 1);
			break ;
		}
		(*i)++;
	}
	pthread_mutex_unlock(philo->start_lock);
}
