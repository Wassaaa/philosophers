/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:25:09 by aklein            #+#    #+#             */
/*   Updated: 2024/04/30 10:32:16 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

void	close_sems(t_philo *philo)
{
	if (philo->death && philo->death != SEM_FAILED)
		sem_close(philo->death);
	if (philo->forks && philo->forks != SEM_FAILED)
		sem_close(philo->forks);
	if (philo->zen && philo->zen != SEM_FAILED)
		sem_close(philo->zen);
	if (philo->lock && philo->lock != SEM_FAILED)
		sem_close(philo->lock);
	if (philo->print && philo->print != SEM_FAILED)
		sem_close(philo->print);
	sem_unlink(FORKS);
	sem_unlink(DEATH);
	sem_unlink(ZEN);
	sem_unlink(LOCK);
	sem_unlink(PRINT);
}

void	free_and_null(void	**freeable)
{
	if (freeable && *freeable)
	{
		free(*freeable);
		*freeable = NULL;
	}
}

void	handle_waitress(t_philo *philo)
{
	int	i;

	if (philo->food != 0 && philo->waitress)
	{
		i = 0;
		while (i++ < philo->num_philos)
			unlock_sem(philo, philo->zen);
		pthread_join(philo->waitress, NULL);
	}
}

void	exit_threads(t_philo *philo)
{
	if (philo->death_watcher)
		pthread_join(philo->death_watcher, NULL);
	if (philo->vitality)
		pthread_join(philo->vitality, NULL);
	if (philo->waitress)
		handle_waitress(philo);
}

void	error(t_philo *philo, int ret, char *msg)
{
	if (philo->death)
		unlock_sem(philo, philo->death);
	exit_threads(philo);
	if (philo->lock)
	{
		lock_sem(philo, philo->lock);
		free_and_null((void **)&philo->pids);
		free_and_null((void **)&philo->die);
		free_and_null((void **)&philo->fed);
		unlock_sem(philo, philo->lock);
	}
	close_sems(philo);
	if (msg)
		printf("%s\n", msg);
	exit(ret);
}

void	pre_life(t_philo *philo)
{
	if (gettimeofday(&philo->start, NULL) == -1)
		error(philo, 1, ERR_TIME);
	lock_sem(philo, philo->lock);
	philo->fed = malloc(sizeof(struct timeval));
	if (!philo->fed)
		error(philo, 1, ERR_MALLOC);
	*philo->fed = philo->start;
	philo->die = malloc(sizeof(int));
	if (!philo->die)
		error(philo, 1, ERR_MALLOC);
	*philo->die = 0;
	unlock_sem(philo, philo->lock);
}

void	existential_meal(t_philo *philo)
{
	lock_sem(philo, philo->forks);
	print_message(FORK, philo);
	if (philo->num_philos == 1)
	{
		sentient_pause(philo->to_die, philo);
		lock_sem(philo, philo->lock);
		(*philo->die)++;
		unlock_sem(philo, philo->lock);
		print_message(DIE, philo);
		unlock_sem(philo, philo->print);
		return ;
	}
	lock_sem(philo, philo->forks);
	print_message(FORK, philo);
	print_message(EAT, philo);
	lock_sem(philo, philo->lock);
	if (gettimeofday(philo->fed, NULL) == -1)
		error(philo, 1, ERR_TIME);
	unlock_sem(philo, philo->lock);
	sentient_pause(philo->to_eat, philo);
	print_message(SLEEP, philo);
	if (--philo->food == 0)
		unlock_sem(philo, philo->zen);
}

void	existential_cycle(t_philo *philo)
{
	while (verify_existence(philo))
	{
		existential_meal(philo);
		unlock_sem(philo, philo->forks);
		unlock_sem(philo, philo->forks);
		sentient_pause(philo->to_sleep, philo);
		print_message(THINK, philo);
	}
}

void	*waitress(void *p)
{
	t_philo	*philo;
	int		zen_reached;

	philo = (t_philo *)p;
	zen_reached = 0;
	while (zen_reached <= philo->num_philos)
	{
		lock_sem(philo, philo->zen);
		if (++zen_reached >= philo->num_philos)
		{
			unlock_sem(philo, philo->death);
			lock_sem(philo, philo->print);
			printf("all philos have eaten at least %d meals\n", philo->food);
			usleep(10000);
			unlock_sem(philo, philo->print);
			break ;
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
	{
		if (!sentient_pause(1, philo))
			break ;
	}
	return (NULL);
}

void	*death_watcher(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	lock_sem(philo, philo->death);
	lock_sem(philo, philo->lock);
	*philo->die = 2;
	unlock_sem(philo, philo->lock);
	unlock_sem(philo, philo->death);
	return (NULL);
}

void	start_philo(t_philo *philo)
{
	pre_life(philo);
	if (pthread_create(&philo->death_watcher, NULL, &death_watcher, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
	if (pthread_create(&philo->vitality, NULL, &vitality_checker, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
	existential_cycle(philo);
	error(philo, 0, NULL);
}

void	call_waitress(t_philo *philo)
{
	if (pthread_create(&philo->waitress, NULL, &waitress, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
}

void	start_philos(t_philo *philo)
{
	int	i;

	i = 0;
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
		usleep(100);
		i++;
	}
	if (philo->food != 0)
		call_waitress(philo);
	while (i--)
		waitpid(philo->pids[i], NULL, 0);
	error(philo, 0, NULL);
}

void	start_sem(t_philo *philo)
{
	sem_unlink(FORKS);
	sem_unlink(DEATH);
	sem_unlink(ZEN);
	sem_unlink(LOCK);
	sem_unlink(PRINT);
	philo->forks = sem_open(FORKS, O_CREAT, 0600, philo->num_philos);
	if (philo->forks == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->death = sem_open(DEATH, O_CREAT, 0600, 0);
	if (philo->death == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->zen = sem_open(ZEN, O_CREAT, 0600, 0);
	if (philo->zen == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->lock = sem_open(LOCK, O_CREAT, 0600, 1);
	if (philo->lock == SEM_FAILED)
		error(philo, 1, ERR_SEM_OPEN);
	philo->print = sem_open(PRINT, O_CREAT, 0600, 1);
	if (philo->print == SEM_FAILED)
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
