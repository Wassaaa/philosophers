/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sem_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:40:17 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 11:40:18 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

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