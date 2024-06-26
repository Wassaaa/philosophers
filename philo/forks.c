/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:43:50 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 17:08:19 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

static int	forks_avaliable(t_philo *philo)
{
	int	i;
	int	sum;

	i = 0;
	sum = 0;
	lock_mutex(philo, philo->fork_lock);
	while (i < philo->num_philos)
	{
		if (philo->fork_states[i++])
			sum++;
	}
	unlock_mutex(philo, philo->fork_lock);
	return (philo->num_philos - sum);
}

static int	try_fork(t_philo *philo, int fork)
{
	lock_mutex(philo, philo->fork_lock);
	if (!philo->fork_states[fork])
	{
		philo->fork_states[fork] = 1;
		lock_mutex(philo, &philo->forks[fork]);
		unlock_mutex(philo, philo->fork_lock);
		print_message(FORK, philo);
		return (1);
	}
	unlock_mutex(philo, philo->fork_lock);
	return (0);
}

static int	double_fork(t_philo *philo, int left, int right)
{
	lock_mutex(philo, philo->fork_lock);
	if (!philo->fork_states[left] && !philo->fork_states[right])
	{
		philo->fork_states[left] = 1;
		philo->fork_states[right] = 1;
		philo->left_fork = 1;
		philo->right_fork = 1;
		lock_mutex(philo, &(philo->forks[left]));
		lock_mutex(philo, &(philo->forks[right]));
		unlock_mutex(philo, philo->fork_lock);
		print_message(FORK, philo);
		print_message(FORK, philo);
		return (1);
	}
	unlock_mutex(philo, philo->fork_lock);
	return (0);
}

int	get_forks(t_philo *philo, int left, int right)
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
	return (double_fork(philo, left, right));
}
