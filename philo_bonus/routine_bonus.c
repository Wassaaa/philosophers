/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:22:09 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 16:46:36 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static void	existential_meal(t_philo *philo)
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
	if (philo->id % 2 == 0)
	{
		print_message(THINK, philo);
		sentient_pause(philo->to_eat - 5, philo);
	}
	while (verify_existence(philo))
	{
		existential_meal(philo);
		unlock_sem(philo, philo->forks);
		unlock_sem(philo, philo->forks);
		sentient_pause(philo->to_sleep, philo);
		print_message(THINK, philo);
	}
}
