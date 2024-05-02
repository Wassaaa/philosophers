/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:33:19 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 17:09:58 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static void	existential_disengagement(t_philo *philo)
{
	int	right_fork;
	int	left_fork;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % philo->num_philos;
	if (philo->left_fork)
	{
		unlock_mutex(philo, &(philo->forks[left_fork]));
		philo->left_fork = 0;
		lock_mutex(philo, philo->fork_lock);
		philo->fork_states[left_fork] = 0;
		unlock_mutex(philo, philo->fork_lock);
	}
	if (philo->right_fork)
	{
		philo->right_fork = 0;
		unlock_mutex(philo, &(philo->forks[right_fork]));
		lock_mutex(philo, philo->fork_lock);
		philo->fork_states[right_fork] = 0;
		unlock_mutex(philo, philo->fork_lock);
	}
}

static int	existential_grasp(t_philo *philo)
{
	int	right_fork;
	int	left_fork;

	left_fork = philo->id;
	right_fork = (philo->id + 1) % philo->num_philos;
	while (!halt_manager(philo, 0))
	{
		if (!verify_existence(philo))
		{
			print_message(DIE, philo);
			break ;
		}
		if (get_forks(philo, left_fork, right_fork))
			break ;
		usleep(100);
	}
	if (halt_manager(philo, 0))
		return (0);
	return (1);
}

static int	existential_meal(t_philo *philo)
{
	if (!existential_grasp(philo))
	{
		existential_disengagement(philo);
		return (0);
	}
	get_time(philo, &philo->fed);
	print_message(EAT, philo);
	if (!sentient_pause(philo->to_eat, philo))
		print_message(DIE, philo);
	if (--philo->food == 0)
	{
		lock_mutex(philo, philo->food_lock);
		(*philo->food_finished)++;
		unlock_mutex(philo, philo->food_lock);
	}
	return (1);
}

void	*existential_cycle(void *p)
{
	t_philo	*philo;

	philo = (t_philo *)p;
	lock_mutex(philo, philo->start_lock);
	get_time(philo, &philo->start);
	unlock_mutex(philo, philo->start_lock);
	philo->fed = philo->start;
	if (philo->id % 2 == 0)
	{
		print_message(THINK, philo);
		sentient_pause(philo->to_eat - 5, philo);
	}
	while (!halt_manager(philo, 0))
	{
		if (!existential_meal(philo))
			break ;
		print_message(SLEEP, philo);
		existential_disengagement(philo);
		if (!sentient_pause(philo->to_sleep, philo))
			print_message(DIE, philo);
		print_message(THINK, philo);
	}
	free(philo);
	return (NULL);
}
