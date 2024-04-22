/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:57:42 by aklein            #+#    #+#             */
/*   Updated: 2024/04/23 00:57:45 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <philo.h>
#include <unistd.h>

void	error()
{
	exit(1);
}

void	*existential_cycle(void *p)
{
	t_philo	philo;

	philo = *(t_philo *)p;
	if (philo.id < 3)
		sleep(1);
	printf("philo %d started\n", philo.id + 1);
	free(p);
	return (NULL);
}

void	init_forks(t_philo *set)
{
	int		i;

	i = 0;
	set->forks = malloc(set->num_philos * sizeof(pthread_mutex_t));
	if (!set->forks)
		error();
	while (i < set->num_philos)
	{
		pthread_mutex_init(&set->forks[i++], NULL);
	}
}

pthread_t	launch_philo(t_philo philo)
{
	pthread_t	thread;
	t_philo		*p;

	p = malloc(sizeof(t_philo));
	*p = philo;
	pthread_create(&thread, NULL, &existential_cycle, p);
	return (thread);
}

int	main(int argc, char **argv)
{
	t_philo		philo;
	pthread_t	*threads;
	int			i;

	i = 0;
	if (argc == 5)
	{
		philo.num_philos = ft_atoi(argv[1]);
		philo.to_die = ft_atoi(argv[2]);
		philo.to_eat = ft_atoi(argv[3]);
		philo.to_sleep = ft_atoi(argv[4]);
		threads = malloc(philo.num_philos * sizeof(pthread_t));
		init_forks(&philo);
		while (i < philo.num_philos)
		{
			philo.id = i;
			threads[i] = launch_philo(philo);
			i++;
		}
		i = 0;
		while (i < philo.num_philos)
		{
			pthread_join(threads[i], NULL);
			i++;
		}
	}
	return (0);
}