/*
** utils.c for Project in /home/enguerrand/delivery/Project/utils.c
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  Sun Oct 29 16:51:38 2017 Enguerrand Allamel
** Last update Sun Oct 29 16:51:38 2017 Enguerrand Allamel
*/

#include "pamela.h"

/**
 * Get home for dir to username
 * @param user username
 * @return char *home_dir (home dir path) / NULL (if not found)
 */
const char 		*get_user_home(const char *user)
{
  struct passwd	*passwdEnt;
  char 			*user_home;

  if (((passwdEnt = getpwnam(user)) == NULL) ||
	  ((user_home = passwdEnt->pw_dir) == NULL))
	return (NULL);
  return (user_home);
}