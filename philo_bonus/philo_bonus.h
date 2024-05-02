/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aklein <aklein@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 00:20:36 by aklein            #+#    #+#             */
/*   Updated: 2024/05/02 16:31:48 by aklein           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>

# include <sys/types.h>
# include <sys/wait.h>

# define FORKS "/forks"
# define PRINT "/print"
# define DEATH "/death"
# define ZEN "/zen"
# define LOCK "/lock"

# define ERR_SEM_OPEN "ERROR: sem_open failed"
# define ERR_SEM_CLOSE "ERROR: sem_close failed"
# define ERR_SEM_WAIT "ERROR: sem_wait failed"
# define ERR_SEM_POST "ERROR: sem_post failed"
# define ERR_TIME "ERROR: gettimeofday failed"
# define ERR_FORK "ERROR: Fork failed"
# define ERR_SIGNAL "ERROR: Sending signal failed"
# define ERR_THREAD_C "ERROR: Thread creation failed"
# define ERR_THREAD_J "ERROR: Thread join failed"
# define ERR_MALLOC "ERROR: Malloc failed"

typedef struct s_philo
{
	int				id;
	int				*die;
	pthread_t		death_watcher;
	pthread_t		vitality;
	pthread_t		waitress;
	int				num_philos;
	pid_t			*pids;
	int				to_die;
	int				to_eat;
	int				to_sleep;
	int				food;
	sem_t			*forks;
	sem_t			*death;
	sem_t			*zen;
	sem_t			*lock;
	sem_t			*print;
	struct timeval	start;
	struct timeval	*fed;
}					t_philo;

typedef enum e_msg
{
	FORK,
	EAT,
	SLEEP,
	THINK,
	DIE
}	t_msg;

void	check_args(int argc, char **argv);
void	get_args(t_philo *philo, int argc, char **argv);
void	start_sem(t_philo *philo);
void	start_philos(t_philo *philo);
int		sentient_pause(int ms, t_philo *philo);
int		verify_existence(t_philo *philo);
void	existential_cycle(t_philo *philo);
void	call_waitress(t_philo *philo);
void	error(t_philo *philo, int ret, char *msg);
void	lock_sem(t_philo *philo, sem_t *sem);
void	unlock_sem(t_philo *philo, sem_t *sem);
int		ft_atoi(const char *str);
int		get_ms(t_philo *philo, struct timeval start);
void	print_message(t_msg msg, t_philo *philo);

#endif