/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:57:42 by aklein            #+#    #+#             */
/*   Updated: 2024/04/26 01:29:58 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <philo.h>
#include <unistd.h>

int	verify_existence(t_philo *philo);

void	destroy_mutex(pthread_mutex_t *mutex)
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

int	halt_manager(t_philo *philo, int to_halt)
{
	int status;

	if (to_halt)
	{
		pthread_mutex_lock(philo->halt_lock);
		*philo->halt_deliberation = 1;
		pthread_mutex_unlock(philo->halt_lock);
		status = 1;
	}
	else
	{
		pthread_mutex_lock(philo->halt_lock);
		status = *philo->halt_deliberation;
		pthread_mutex_unlock(philo->halt_lock);
	}
	return (status);
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
	if (!halt_manager(philo, 0))
	{
		pthread_mutex_lock(philo->print_lock);
		if (halt_manager(philo, 0))
		{
			pthread_mutex_unlock(philo->print_lock);
			return ;
		}
		if (msg == DIE || !verify_existence(philo))
		{
			printf("%d %d died (%d : %d)\n",get_ms(philo->start), philo->id + 1, get_ms(philo->fed), philo->to_die);
			//printf("%d %d died\n",get_ms(philo->start), philo->id + 1);
			halt_manager(philo, 1);
			pthread_mutex_unlock(philo->print_lock);
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
		pthread_mutex_unlock(philo->print_lock);
	}
}

int	verify_existence(t_philo *philo)
{
	if (get_ms(philo->fed) >= philo->to_die)
		return (0);
	return (1);
}

int	forks_avaliable(t_philo *philo)
{
	int	i;
	int	sum;

	i = 0;
	sum = 0;
	pthread_mutex_lock(philo->fork_lock);
	while (i < philo->num_philos)
	{
		if (philo->fork_states[i++])
			sum++;
	}
	pthread_mutex_unlock(philo->fork_lock);
	return (philo->num_philos - sum);
}

int	try_fork(t_philo *philo, int fork)
{
	pthread_mutex_lock(philo->fork_lock);
	if (!philo->fork_states[fork])
	{
		philo->fork_states[fork] = 1;
		pthread_mutex_lock(&philo->forks[fork]);
		pthread_mutex_unlock(philo->fork_lock);
		print_message(FORK, philo);
		return (1);
	}
	pthread_mutex_unlock(philo->fork_lock);
	return (0);
}

int	try_two_forks(t_philo *philo, int left, int right)
{
	if (left == right)
		return (0);
	if (philo->num_philos % 2 == 1 && forks_avaliable(philo) == 1)
	{
		while (verify_existence(philo) && !halt_manager(philo, 0))
		{
			if (try_fork(philo, left))
				philo->left_fork = 1;
			if (try_fork(philo, right))
				philo->right_fork = 1;
			if (philo->right_fork && philo->left_fork)
				return (1);
			usleep(100);
		}
	}
	pthread_mutex_lock(philo->fork_lock);
	if (!philo->fork_states[left] && !philo->fork_states[right])
	{
		philo->fork_states[left] = 1;
		philo->fork_states[right] = 1;
		philo->left_fork = 1;
		philo->right_fork = 1;
		pthread_mutex_unlock(philo->fork_lock);
		pthread_mutex_lock(&(philo->forks[left]));
		pthread_mutex_lock(&(philo->forks[right]));
		print_message(FORK, philo);
		print_message(FORK, philo);
		return (1);
	}
	pthread_mutex_unlock(philo->fork_lock);
	return (0);
}

int	existential_grasp(t_philo *philo)
{
	int		right_fork;
	int		left_fork;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % philo->num_philos;
	while (!halt_manager(philo, 0))
	{
		if (!verify_existence(philo))
		{
			print_message(DIE, philo);
			break ;
		}
		if (try_two_forks(philo, left_fork, right_fork))
			break ;
		usleep(100);
	}
	if (halt_manager(philo, 0))
		return (0);
	return (1);
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

void	existential_disengagement(t_philo *philo)
{
	int		right_fork;
	int		left_fork;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % philo->num_philos;
	if (philo->left_fork)
	{
		pthread_mutex_unlock(&(philo->forks[left_fork]));
		philo->left_fork = 0;
		pthread_mutex_lock(philo->fork_lock);
		philo->fork_states[left_fork] = 0;
		pthread_mutex_unlock(philo->fork_lock);
	}
	if (philo->right_fork)
	{
		philo->right_fork = 0;
		pthread_mutex_unlock(&(philo->forks[right_fork]));
		pthread_mutex_lock(philo->fork_lock);
		philo->fork_states[right_fork] = 0;
		pthread_mutex_unlock(philo->fork_lock);
	}
}

int	existential_meal(t_philo *philo)
{
	if (!existential_grasp(philo))
	{
		existential_disengagement(philo);
		return (0);
	}
	gettimeofday(&philo->fed, NULL);
	print_message(EAT, philo);
	if (!sentient_pause(philo->to_eat, philo))
		print_message(DIE, philo);
	if (--philo->food == 0)
	{
		pthread_mutex_lock(philo->food_lock);
		(*philo->food_finished)++;
		pthread_mutex_unlock(philo->food_lock);
	}
	existential_disengagement(philo);
	return (1);
}

void	*existential_cycle(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	pthread_mutex_lock(philo->start_lock);
	gettimeofday(&philo->start, NULL);
	pthread_mutex_unlock(philo->start_lock);
	philo->fed = philo->start;
	if (philo->id % 2 == 1)
		sentient_pause(1, philo);
	while (!halt_manager(philo, 0))
	{
		if (!existential_meal(philo))
			break ;
		print_message(SLEEP, philo);
		if (!sentient_pause(philo->to_sleep, philo))
			print_message(DIE, philo);
		print_message(THINK, philo);
	}
	free(philo);
	return (NULL);
}

int	init_forks(t_philo *philo)
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

pthread_t	launch_philo(t_philo philo)
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

void	get_args(t_philo *philo, int argc, char **argv)
{
	philo->num_philos = ft_atoi(argv[1]);
	philo->to_die = ft_atoi(argv[2]);
	philo->to_eat = ft_atoi(argv[3]);
	philo->to_sleep = ft_atoi(argv[4]);
	philo->food = 0;
	if (argc == 6)
		philo->food = ft_atoi(argv[5]);
}

int	init_locks(t_philo *philo)
{
	philo->print_lock = malloc(sizeof(pthread_mutex_t));
	if (!philo->print_lock)
		return (0);
	philo->fork_lock = malloc(sizeof(pthread_mutex_t));
	if (!philo->fork_lock)
		return (0);
	philo->halt_lock = malloc(sizeof(pthread_mutex_t));
	if (!philo->halt_lock)
		return (0);
	philo->food_lock = malloc(sizeof(pthread_mutex_t));
	if (!philo->food_lock)
		return (0);
	philo->start_lock = malloc(sizeof(pthread_mutex_t));
	if (!philo->start_lock)
		return (0);
	if (pthread_mutex_init(philo->print_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(philo->fork_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(philo->halt_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(philo->food_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(philo->start_lock, NULL) != 0)
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
	philo->right_fork = 0;
	philo->left_fork = 0;
	return (1);
}

void	zen_monitor(t_philo *philo)
{
	while (!halt_manager(philo, 0))
	{
		pthread_mutex_lock(philo->food_lock);
		if (*philo->food_finished == philo->num_philos)
		{
			halt_manager(philo, 1);
			pthread_mutex_unlock(philo->food_lock);
			return ;	
		}
		pthread_mutex_unlock(philo->food_lock);
		usleep(1000);
	}
}

int	main(int argc, char **argv)
{
	t_philo		philo;
	int			i;

	i = 0;
	if (argc >= 5)
	{
		get_args(&philo, argc, argv);
		philo.threads = (pthread_t *)malloc(philo.num_philos * sizeof(pthread_t));
		if (!philo.threads)
			return (1);
		if (init_struct(&philo))
		{
			pthread_mutex_lock(philo.start_lock);
			while (i < philo.num_philos)
			{
				philo.id = i;
				philo.threads[i] = launch_philo(philo);
				if (philo.threads[i] == 0)
				{
					halt_manager(&philo, 1);
					break ;
				}
				i++;
			}
			pthread_mutex_unlock(philo.start_lock);
		}
		if (i == philo.num_philos)
			zen_monitor(&philo);
		while (i--)
			pthread_join(philo.threads[i], NULL);
		free_all(&philo);
	}
	return (0);
}
