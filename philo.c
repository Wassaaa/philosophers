/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:57:42 by aklein            #+#    #+#             */
/*   Updated: 2024/04/24 02:19:30 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <philo.h>
#include <unistd.h>

int	verify_existence(t_philo *philo);

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
	if (!*philo->halt_deliberation)
	{
		pthread_mutex_lock(philo->print_lock);
		if (*philo->halt_deliberation)
		{
			pthread_mutex_unlock(philo->print_lock);
			return ;
		}
		if (msg == DIE || !verify_existence(philo))
		{
			printf("%d %d died (%d : %d)\n",get_ms(philo->start), philo->id + 1, get_ms(philo->fed), philo->to_die);
			//printf("%d %d died\n",get_ms(philo->start), philo->id + 1);
			*philo->halt_deliberation = 1;
			return ;
		}
		if (msg == FORK)
			printf("%d %d has taken a fork\n", get_ms(philo->start), philo->id + 1);
		if (msg == EAT)
			printf("%d %d is eating\n", get_ms(philo->start), philo->id + 1);
		if (msg == SLEEP)
			printf("%d %d is sleeping\n", get_ms(philo->start), philo->id + 1);
		if (msg == THINK)
			printf("%d %d is thinking\n", get_ms(philo->start), philo->id + 1);
		if (!*philo->halt_deliberation)
			pthread_mutex_unlock(philo->print_lock);
	}
}

int	verify_existence(t_philo *philo)
{
	if (get_ms(philo->fed) >= philo->to_die)
		return (0);
	return (1);
}

int	try_two_forks(t_philo *philo, int left, int right)
{
	pthread_mutex_lock(philo->fork_lock);
	if (!philo->fork_states[left] && !philo->fork_states[right])
	{
		philo->fork_states[left] = 1;
		philo->fork_states[right] = 1;
		pthread_mutex_unlock(philo->fork_lock);
		pthread_mutex_lock(&(philo->forks[left]));
		pthread_mutex_lock(&(philo->forks[right]));
		return (1);
	}
	pthread_mutex_unlock(philo->fork_lock);
	return (0);
}

void	handle_forks(t_philo *philo)
{
	int		right_fork;
	int		left_fork;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % philo->num_philos;
	while (!*philo->halt_deliberation)
	{
		if (!verify_existence(philo))
			print_message(DIE, philo);
		if (try_two_forks(philo, left_fork, right_fork))
		{
			print_message(FORK, philo);
			print_message(FORK, philo);
			break ;
		}
	}
}

// void	handle_forks(t_philo *philo)
// {
// 	int		right_fork;
// 	int		left_fork;

// 	left_fork = philo->id;
// 	right_fork = (philo->id + 1) % philo->num_philos;
// 	while (!*philo->halt_deliberation)
// 	{
// 		if (!verify_existence(philo))
// 			print_message(DIE, philo);
// 		if (!philo->fork_states[right_fork] && !philo->fork_states[right_fork])
// 		{
// 			if (philo->id % 2 == 0)
// 			{
// 				pthread_mutex_lock(&(philo->forks[right_fork]));
// 				philo->fork_states[right_fork] = 1;
// 				print_message(FORK, philo);
// 				pthread_mutex_lock(&(philo->forks[left_fork]));
// 				philo->fork_states[left_fork] = 1;
// 				print_message(FORK, philo);
// 				break ;
// 			}
// 			else
// 			{
// 				pthread_mutex_lock(&(philo->forks[left_fork]));
// 				philo->fork_states[left_fork] = 1;
// 				print_message(FORK, philo);
// 				pthread_mutex_lock(&(philo->forks[right_fork]));
// 				philo->fork_states[right_fork] = 1;
// 				print_message(FORK, philo);
// 				break ;
// 			}
// 		}
// 	}
// }

// void	handle_forks(t_philo *philo)
// {
// 	int		right_fork;
// 	int		left_fork;

// 	left_fork = philo->id;
// 	right_fork = (philo->id + 1) % philo->num_philos;
// 	if (philo->id % 2 == 0)
// 	{
// 		pthread_mutex_lock(&(philo->forks[right_fork]));
// 		philo->fork_states[right_fork] = 1;
// 		print_message(FORK, philo);
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(&(philo->forks[left_fork]));
// 		philo->fork_states[left_fork] = 1;
// 		print_message(FORK, philo);
// 	}
// 	if (philo->id % 2 != 0)
// 	{
// 		pthread_mutex_lock(&(philo->forks[right_fork]));
// 		philo->fork_states[right_fork] = 1;
// 		print_message(FORK, philo);
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(&(philo->forks[left_fork]));
// 		philo->fork_states[left_fork] = 1;
// 		print_message(FORK, philo);
// 	}
// }

int	watchful_slumber(int ms, t_philo *philo)
{
	struct timeval	start;
	struct timeval	current;
	int				elapsed;

	gettimeofday(&start, NULL);
	elapsed = 0;
	while (elapsed < ms)
	{
		if (*philo->halt_deliberation || !verify_existence(philo))
			return (0);
		usleep(1000);
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000;
		elapsed += (current.tv_usec - start.tv_usec) / 1000;
	}
	return (1);
}

void	*existential_cycle(void *p)
{
	t_philo	*philo;
	int		right_fork;
	int		left_fork;

	philo = (t_philo *)p;
	left_fork = philo->id;
	right_fork = (philo->id + 1) % philo->num_philos;
	while (!*philo->halt_deliberation)
	{
		handle_forks(philo);
		gettimeofday(&philo->fed, NULL);
		philo->food--;
		print_message(EAT, philo);
		if (!watchful_slumber(philo->to_eat, philo))
			print_message(DIE, philo);
		pthread_mutex_unlock(&(philo->forks[left_fork]));
		pthread_mutex_unlock(&(philo->forks[right_fork]));
		pthread_mutex_lock(philo->fork_lock);
		philo->fork_states[left_fork] = 0;
		philo->fork_states[right_fork] = 0;
		pthread_mutex_unlock(philo->fork_lock);
		print_message(SLEEP, philo);
		if (!watchful_slumber(philo->to_sleep, philo))
			print_message(DIE, philo);
		print_message(THINK, philo);
	}
	// if (food >= 5)
	// 	printf("%d %d is done\n",get_ms(philo->start), philo->id + 1);
	free(philo);
	return (NULL);
}

void	init_forks(t_philo *philo)
{
	int		i;

	i = 0;
	philo->forks = malloc(philo->num_philos * sizeof(pthread_mutex_t));
	philo->fork_states = malloc (philo->num_philos * sizeof(int));
	if (!philo->forks)
		error();
	while (i < philo->num_philos)
	{
		philo->fork_states[i] = 0;
		pthread_mutex_init(&philo->forks[i++], NULL);
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

void	init_struct(t_philo *philo, int argc, char **argv)
{
		philo->num_philos = ft_atoi(argv[1]);
		philo->to_die = ft_atoi(argv[2]);
		philo->to_eat = ft_atoi(argv[3]);
		philo->to_sleep = ft_atoi(argv[4]);
		if (argc == 6)
			philo->food = ft_atoi(argv[5]);
		philo->print_lock = malloc(sizeof(pthread_mutex_t));
		philo->fork_lock = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo->print_lock, NULL);
		pthread_mutex_init(philo->fork_lock, NULL);
		init_forks(philo);
		philo->halt_deliberation = malloc(sizeof(int));
		*philo->halt_deliberation = 0;
}

int	main(int argc, char **argv)
{
	t_philo		philo;
	pthread_t	*threads;
	int			i;

	i = 0;
	if (argc >= 5)
	{
		init_struct(&philo, argc, argv);
		threads = (pthread_t *)malloc(philo.num_philos * sizeof(pthread_t));		
		while (i < philo.num_philos)
		{
			philo.id = i;
			threads[i] = launch_philo(philo);
			i++;
		}
		i = 0;
		while (i < philo.num_philos)
			pthread_join(threads[i++], NULL);
		i = 0;
		while (i < philo.num_philos)
			pthread_mutex_destroy(&philo.forks[i++]);
		pthread_mutex_destroy(philo.print_lock);
		free(philo.print_lock);
		free(philo.forks);
	}
	return (0);
}