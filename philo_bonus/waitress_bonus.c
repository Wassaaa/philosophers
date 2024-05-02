/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitress_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 11:39:40 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 11:49:43 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo_bonus.h>

static void	*waitress(void *p)
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

void	call_waitress(t_philo *philo)
{
	if (pthread_create(&philo->waitress, NULL, &waitress, philo) != 0)
		error(philo, 1, ERR_THREAD_C);
}