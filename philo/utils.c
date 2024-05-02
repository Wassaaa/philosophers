/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 06:31:04 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 08:13:46 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

void	*my_mal(t_philo *philo, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		log_err(philo, ERR_MALLOC);
	return (ptr);
}

int	lock_mutex(t_philo *philo, pthread_mutex_t *mut)
{
	if (pthread_mutex_lock(mut) != 0)
	{
		log_err(philo, ERR_MUT_LOCK);
		halt_manager(philo, 1);
		return (1);
	}
	return (0);
}

int	unlock_mutex(t_philo *philo, pthread_mutex_t *mut)
{
	if (pthread_mutex_unlock(mut) != 0)
	{
		log_err(philo, ERR_MUT_UNLOCK);
		halt_manager(philo, 1);
		return (1);
	}
	return (0);
}