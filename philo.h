/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:55:55 by aklein            #+#    #+#             */
/*   Updated: 2024/04/26 05:16:27 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <string.h>

typedef struct s_philo
{
	pthread_t		*threads;
	int				id;
	int				num_philos;
	int				to_die;
	int				to_eat;
	int				to_sleep;
	int				food;
	int				right_fork;
	int				left_fork;
	struct timeval	fed;
	struct timeval	start;
	int				*halt_deliberation;
	int				*fork_states;
	int				*food_finished;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*fork_lock;
	pthread_mutex_t	*halt_lock;
	pthread_mutex_t	*food_lock;
	pthread_mutex_t	*start_lock;
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