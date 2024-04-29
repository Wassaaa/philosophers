/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:25:09 by aklein            #+#    #+#             */
/*   Updated: 2024/04/30 02:17:53 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	close_sems(t_philo *philo)
{
	if (philo->death)
	{
		sem_post(philo->death);
		sem_close(philo->death);
	}
	if (philo->forks)
		sem_close(philo->forks);
	if (philo->print)
		sem_close(philo->print);
	if (philo->zen)
		sem_close(philo->zen);
	if (philo->lock)
		sem_close(philo->lock);
	sem_unlink(FORKS);
	sem_unlink(PRINT);
	sem_unlink(DEATH);
	sem_unlink(ZEN);
	sem_unlink(FED_LOCK);
}

void	error(t_philo *philo, int ret, char *msg)
{
	if (msg)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	if (philo->lock)
	{
		if (sem_wait(philo->lock) == -1)
			error(philo, 1, ERR_SEM_WAIT);
		if (philo->pids)
		{
			free(philo->pids);
			philo->pids = NULL;
		}
		if (sem_post(philo->lock) == -1)
			error(philo, 1, ERR_SEM_POST);
	}
	close_sems(philo);
	exit(ret);
}

void	pre_life(t_philo *philo)
{
	if (gettimeofday(&philo->start, NULL) == -1)
		error(philo, 1, ERR_TIME);
	get_fed(philo, 0);
}

void	existential_meal(t_philo *philo)
{
	if (sem_wait(philo->forks) == -1)
		error(philo, 1, ERR_SEM_WAIT);
	print_message(FORK, philo);
	if (philo->num_philos == 1)
		sentient_pause(philo->to_die * 2, philo);
	if (sem_wait(philo->forks) == -1)
		error(philo, 1, ERR_SEM_WAIT);
	print_message(FORK, philo);
	print_message(EAT, philo);
	get_fed(philo, 0);
	sentient_pause(philo->to_eat, philo);
	print_message(SLEEP, philo);
	if (--philo->food == 0)
	{
		if (sem_post(philo->zen) == -1)
			error(philo, 1, ERR_SEM_POST);
	}
}

void	existential_cycle(t_philo *philo)
{
	pre_life(philo);
	while (42)
	{
		existential_meal(philo);
		if (sem_post(philo->forks) == -1)
			error(philo, 1, ERR_SEM_POST);
		if (sem_post(philo->forks) == -1)
			error(philo, 1, ERR_SEM_POST);
		sentient_pause(philo->to_sleep, philo);
		print_message(THINK, philo);
	}
}

void	*waitress(void *p)
{
	t_philo	*philo;
	int	zen_reached;

	philo = (t_philo *)p;
	zen_reached = 0;
	while (42)
	{
		sem_wait(philo->zen);
		if (++zen_reached >= philo->num_philos)
		{
			sem_wait(philo->print);
			ft_putstr_fd("all philos have eaten at least ", 1);
			ft_putnbr_fd(philo->food, 1);
			ft_putstr_fd(" meals\n", 1);
			error(philo, 0, NULL);
		}
	}
	return (NULL);
}

void	*vitality_checker(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	usleep(philo->to_die);
	while (42)
		sentient_pause(1, philo);
	return (NULL);
}

void	*death_watcher(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	if (sem_wait(philo->death) == -1)
		error(philo, 1, ERR_SEM_WAIT);
	if (sem_post(philo->death) == -1)
		error(philo, 1, ERR_SEM_POST);
	error(philo, 0, NULL);
	return (NULL);
}

void	start_philo(t_philo *philo)
{
	pthread_t	death;
	pthread_t	vitality;

	if (pthread_create(&death, NULL, &death_watcher, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
	if (pthread_detach(death) != 0)
		error(philo, 1, ERR_THREAD_D);
	if (pthread_create(&vitality, NULL, &vitality_checker, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
	if (pthread_detach(vitality) != 0)
		error(philo, 1, ERR_THREAD_D);
	existential_cycle(philo);
	error(philo, 0, NULL);
}

void	call_waitress(t_philo *philo)
{
	pthread_t	thread;

	if (pthread_create(&thread, NULL, &waitress, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
	if (pthread_detach(thread) != 0)
		error(philo, 1, ERR_THREAD_D);
}

void	start_philos(t_philo *philo)
{
	int	i;

	i = 0;
	if (philo->food != 0)
		call_waitress(philo);
	philo->pids = malloc(philo->num_philos * sizeof(pid_t));
	if (!philo->pids)
		error(philo, 1, ERR_MALLOC);
	while (i < philo->num_philos)
	{
		philo->pids[i] = fork();
		if (philo->pids[i] == -1)
			error(philo, 1, ERR_FORK);
		if (philo->pids[i] == 0)
		{
			philo->id = i;
			start_philo(philo);
		}
		i++;
	}
	while (i--)
		waitpid(philo->pids[i], NULL, 0);
	error(philo, 0, NULL);
}

void	start_sem(t_philo *philo)
{
	sem_unlink(FORKS);
	sem_unlink(PRINT);
	sem_unlink(DEATH);
	sem_unlink(ZEN);
	sem_unlink(FED_LOCK);
	philo->forks = sem_open(FORKS, O_CREAT, 0600, philo->num_philos);
	if (philo->forks == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->print = sem_open(PRINT, O_CREAT, 0600, 1);
	if (philo->print == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->death = sem_open(DEATH, O_CREAT, 0600, 0);
	if (philo->death == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->zen = sem_open(ZEN, O_CREAT, 0600, 0);
	if (philo->zen == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->lock = sem_open(FED_LOCK, O_CREAT, 0600, 1);
	if (philo->lock == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
}

int	main(int argc, char **argv)
{
	t_philo	philo;
	
	if (argc >= 5)
	{
		check_args(argc, argv);
		memset(&philo, 0, sizeof(t_philo));
		get_args(&philo, argc, argv);
		start_sem(&philo);
		start_philos(&philo);
	}
	else
		print_usage();
	return (0);
}
