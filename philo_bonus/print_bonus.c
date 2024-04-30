// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   print_bonus.c                                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/04/26 16:55:35 by aklein            #+#    #+#             */
// /*   Updated: 2024/04/29 00:21:26 by aklein           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include <philo_bonus.h>

// static void	write_it(char *str, int ms, int id)
// {
// 	ft_putnbr_fd(ms, STDOUT_FILENO);
// 	ft_putstr_fd(" ", STDOUT_FILENO);
// 	ft_putnbr_fd(id, STDOUT_FILENO);
// 	ft_putstr_fd(str, STDOUT_FILENO);
// }

static void	print_it(t_msg msg, t_philo *philo)
{
	int	id;
	int	ms;

	if (*philo->die > 1)
		return ;
	id = philo->id + 1;
	ms = get_ms(philo, philo->start);
	if (msg == DIE)
		printf("%d %d died\n", ms, id);
	if (msg == FORK)
		printf("%d %d has taken a fork\n", ms, id);
	if (msg == EAT)
		printf("%d %d is eating\n", ms, id);
	if (msg == SLEEP)
		printf("%d %d is sleeping\n", ms, id);
	if (msg == THINK)
		printf("%d %d is thinking\n", ms, id);
}

// void	print_message(t_msg msg, t_philo *philo)
// {
// 	if (!halt_manager(philo, 0))
// 	{
// 		pthread_mutex_lock(philo->print_lock);
// 		if (halt_manager(philo, 0))
// 		{
// 			pthread_mutex_unlock(philo->print_lock);
// 			return ;
// 		}
// 		if (msg == DIE || !verify_existence(philo))
// 		{
// 			print_it(DIE, philo);
// 			halt_manager(philo, 1);
// 			pthread_mutex_unlock(philo->print_lock);
// 			return ;
// 		}
// 		print_it(msg, philo);
// 		pthread_mutex_unlock(philo->print_lock);
// 	}
// }

void	print_message(t_msg msg, t_philo *philo)
{
	lock_sem(philo, philo->print);
	lock_sem(philo, philo->lock);
	if (*philo->die == 1 && msg == DIE)
	{
		print_it(DIE, philo);
		*philo->die = 2;
		unlock_sem(philo, philo->death);
		unlock_sem(philo, philo->lock);
		return ;
	}
	print_it(msg, philo);
	unlock_sem(philo, philo->lock);
	unlock_sem(philo, philo->print);
}
