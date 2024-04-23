/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:57:42 by aklein            #+#    #+#             */
/*   Updated: 2024/04/23 11:20:23 by aklein           ###   ########.fr       */
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

void	*existential_cycle(void *p)
{
	t_philo	*philo;
	int		right_fork;
	int		left_fork;
	int		food;

	food = 0;
	philo = (t_philo *)p;
	left_fork = philo->id;
	right_fork = (philo->id + 1) % philo->num_philos;
	while (food < 5)
	{
		if (get_ms(philo->fed) >= philo->to_die)
		{
			printf("<%d> %d is dead (%d : %d)\n",get_ms(philo->start), philo->id + 1, get_ms(philo->fed), philo->to_die);
			break ;
		}

		pthread_mutex_lock(&(philo->forks[left_fork]));
		printf("<%d> %d has taken the left fork\n",get_ms(philo->start), philo->id + 1);
		pthread_mutex_lock(&(philo->forks[right_fork]));
		printf("<%d> %d has taken the right fork\n",get_ms(philo->start), philo->id + 1);
		printf("<%d> %d is eating\n",get_ms(philo->start), philo->id + 1);
		gettimeofday(&philo->fed, NULL);
		food++;
		usleep(philo->to_eat * 1000);
		printf("<%d> %d is sleeping\n",get_ms(philo->start), philo->id + 1);
		pthread_mutex_unlock(&(philo->forks[left_fork]));
		pthread_mutex_unlock(&(philo->forks[right_fork]));
		usleep(philo->to_sleep * 1000);
		printf("<%d> %d is thinking\n",get_ms(philo->start), philo->id + 1);
	}
	if (food >= 5)
		printf("<%d> %d is done\n",get_ms(philo->start), philo->id + 1);
	free(philo);
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
		/*
		destroy on failure
		*/
	}
}

pthread_t	launch_philo(t_philo philo)
{
	pthread_t		thread;
	t_philo			*p;
	
	p = malloc(sizeof(t_philo));
	*p = philo;
	gettimeofday(&p->start, NULL);
	p->fed = p->start;
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
			pthread_join(threads[i++], NULL);
		while (i < philo.num_philos)
			pthread_mutex_destroy(&philo.forks[i++]);
		free(philo.forks);
	}
	return (0);
}