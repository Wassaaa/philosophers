/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:55:55 by aklein            #+#    #+#             */
/*   Updated: 2024/04/23 16:06:51 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				philos_started;
	int				*num_philos;
	int				to_die;
	int				to_eat;
	int				to_sleep;
	int				food;
	struct timeval	fed;
	struct timeval	start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
}					t_philo;

typedef enum e_msg
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE
}	t_msg;

int	ft_atoi(const char *str);

#endif