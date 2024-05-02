/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:55:55 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 11:50:51 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

# define ERR_MALLOC "ERROR: malloc error"
# define ERR_MUT_INIT "ERROR: mutex init error"
# define ERR_MUT_DEST "ERROR: mutex destroy error"
# define ERR_MUT_LOCK "ERROR: mutex lock error"
# define ERR_MUT_UNLOCK "ERROR: mutex unlock error"
# define ERR_THR_C "ERROR: thread create error"
# define ERR_THR_J "ERROR: thread join error"
# define ERR_TIME "ERROR: gettimeofday error"

typedef struct s_philo
{
	pthread_t		*threads;
	int				id;
	int				num_philos;
	int				to_die;
	int				to_eat;
	int				to_sleep;
	int				food;
	int				thinking;
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

int		ft_atoi(const char *str);
void	get_args(t_philo *philo, int argc, char **argv);
int		init_struct(t_philo *philo);
void	*existential_cycle(void *p);
void	free_all(t_philo *philo);
int		sentient_pause(int ms, t_philo *philo);
int		get_ms(t_philo *philo, struct timeval start);
int		verify_existence(t_philo *philo);
int		halt_manager(t_philo *philo, int to_halt);
void	start_threads(t_philo *philo, int *i);
int		get_forks(t_philo *philo, int left, int right);
void	print_message(t_msg msg, t_philo *philo);
int		check_args(int argc, char **argv);
void	*my_mal(t_philo *philo, size_t size);
int		unlock_mutex(t_philo *philo, pthread_mutex_t *mut);
int		lock_mutex(t_philo *philo, pthread_mutex_t *mut);
int		log_err(t_philo *philo, char *err);
int		get_time(t_philo *philo, struct timeval *time);

#endif