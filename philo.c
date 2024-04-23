/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:57:42 by aklein            #+#    #+#             */
/*   Updated: 2024/04/23 16:10:18 by aklein           ###   ########.fr       */
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

void	print_message(t_msg msg, t_philo *philo)
{
	pthread_mutex_lock(&philo->print_lock);
	if (msg == FORK)
		printf("%d %d has taken a fork\n",get_ms(philo->start), philo->id + 1);
	if (msg == EAT)
		printf("%d %d is eating\n",get_ms(philo->start), philo->id + 1);
	if (msg == SLEEP)
		printf("%d %d is sleeping\n",get_ms(philo->start), philo->id + 1);
	if (msg == THINK)
		printf("%d %d is thinking\n",get_ms(philo->start), philo->id + 1);
	if (msg == DIE)
	{
		printf("%d %d is dead (%d : %d)\n",get_ms(philo->start), philo->id + 1, get_ms(philo->fed), philo->to_die);
		return ;
	}
}

void	handle_forks(t_philo *philo)
{
	int		right_fork;
	int		left_fork;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % *philo->num_philos;
	if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&(philo->forks[right_fork]));
			print_message(FORK, philo);
			pthread_mutex_unlock(&philo->print_lock);
		}
		pthread_mutex_lock(&(philo->forks[left_fork]));
		print_message(FORK, philo);
		pthread_mutex_unlock(&philo->print_lock);
		if (philo->id % 2 != 0)
		{
			pthread_mutex_lock(&(philo->forks[right_fork]));
			print_message(FORK, philo);
			pthread_mutex_unlock(&philo->print_lock);
		}
		print_message(FORK, philo);
		pthread_mutex_unlock(&philo->print_lock);
}

void	*existential_cycle(void *p)
{
	t_philo	*philo;
	int		right_fork;
	int		left_fork;

	philo = (t_philo *)p;
	left_fork = philo->id;
	right_fork = (philo->id + 1) % *philo->num_philos;
	while (philo->philos_started == *(philo->num_philos))
	{
		if (get_ms(philo->fed) >= philo->to_die)
		{
			*philo->num_philos = *philo->num_philos - 1;
			print_message(DIE, philo);
			break ;
		}
		handle_forks(philo);
		gettimeofday(&philo->fed, NULL);

		philo->food--;
		usleep(philo->to_eat * 1000);
		printf("%d %d is sleeping\n",get_ms(philo->start), philo->id + 1);
		pthread_mutex_unlock(&(philo->forks[left_fork]));
		pthread_mutex_unlock(&(philo->forks[right_fork]));
		usleep(philo->to_sleep * 1000);
		printf("%d %d is thinking\n",get_ms(philo->start), philo->id + 1);
	}
	// if (food >= 5)
	// 	printf("%d %d is done\n",get_ms(philo->start), philo->id + 1);
	free(philo);
	return (NULL);
}

void	init_forks(t_philo *set)
{
	int		i;

	i = 0;
	set->forks = malloc(*(set->num_philos) * sizeof(pthread_mutex_t));
	if (!set->forks)
		error();
	while (i < *(set->num_philos))
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
	if (argc >= 5)
	{
		philo.num_philos = malloc(sizeof(int));
		philo.print_lock = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo.print_lock, NULL);
		*(philo.num_philos) = ft_atoi(argv[1]);
		philo.philos_started = *(philo.num_philos);
		philo.to_die = ft_atoi(argv[2]);
		philo.to_eat = ft_atoi(argv[3]);
		philo.to_sleep = ft_atoi(argv[4]);
		if (argc == 6)
			philo.food = ft_atoi(argv[5]);
		threads = malloc(*(philo.num_philos) * sizeof(pthread_t));
		init_forks(&philo);
		while (i < *(philo.num_philos))
		{
			philo.id = i;
			threads[i] = launch_philo(philo);
			i++;
		}
		i = 0;
		while (i < *(philo.num_philos))
			pthread_join(threads[i++], NULL);
		while (i < *(philo.num_philos))
			pthread_mutex_destroy(&philo.forks[i++]);
		free(philo.forks);
	}
	return (0);
}