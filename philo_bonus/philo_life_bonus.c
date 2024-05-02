/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:20:39 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 17:04:34 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static void	pre_life(t_philo *philo)
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

static void	*vitality_checker(void *p)
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

static void	*death_watcher(void *p)
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

static void	start_philo(t_philo *philo)
{
	pre_life(philo);
	if (pthread_create(&philo->death_watcher, NULL, &death_watcher, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
	if (pthread_create(&philo->vitality, NULL, &vitality_checker, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
	existential_cycle(philo);
	error(philo, 0, NULL);
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
