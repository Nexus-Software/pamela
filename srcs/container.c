/*
** container.c for Project in /home/enguerrand/delivery/Project/container.c
**
** Made by Enguerrand Allamel
** Login   <enguerrand.allamel@epitech.eu>
**
** Started on  Fri Oct 27 12:37:57 2017 Enguerrand Allamel
** Last update Fri Oct 27 12:37:57 2017 Enguerrand Allamel
*/

#include "pamela.h"

/**
 * Run cmd in system
 * @param tmp format
 * @param ... va_args
 * @return
 */
static int   execute_cmd(const char *tmp, ...)
{
  va_list     ap;
  char        cmd[BUFFER_SIZE];

  va_start(ap, tmp);
  if (vsprintf(cmd, tmp, ap) < 0)
	return (-1);
  if (system(cmd) < 0)
	return (-1);
  va_end(ap);
  return (0);
}

/**
 * Create new safe container
 * @param volume_path
 * @param user
 * @param pass
 * @return
 */
static int  create_container(const char *volume_path, const char *user, const char *pass)
{
  if (execute_cmd("dd if=/dev/urandom bs=1M count=%d of=%s", CONTAINER_SIZE, volume_path) != 0 ||
	  secure_volume_format(user, pass) != 0 ||
	  secure_volume_open(user, pass) != 0 ||
	  execute_cmd("mkfs.ext4 /dev/mapper/%s_container", user) != 0 ||
	  execute_cmd("mkdir %s/secure_data-rw", get_user_home(user)) != 0 ||
	  execute_cmd("mount /dev/mapper/%s_container %s/secure_data-rw", user, get_user_home(user)) != 0 ||
	  execute_cmd("chown %s %s/secure_data-rw", user, get_user_home(user)) != 0)
	return (-1);
  return (0);
}

/**
 * Change key of container
 * @param user
 * @param old_mdp
 * @param new_mdp
 * @return
 */
int  change_container_mdp(const char *user, const char *old_mdp, const char *new_mdp)
{
  if (secure_volume_change_mdp(user, old_mdp, new_mdp) != 0)
	return (-1);
  return (0);
}

int  close_container(const char *user)
{
  if (execute_cmd("umount -f %s/secure_data-rw", get_user_home(user)) != 0 ||
	  execute_cmd("cryptsetup luksClose %s_container", user) != 0 ||
	  execute_cmd("rm -rf %s/secure_data-rw", get_user_home(user)) != 0)
	return (-1);
  return (0);
}

/**
 * Open container (open secure + mount + setup access)
 * @param user
 * @param pass
 * @return
 */
int     open_container(const char *user, const char *pass)
{
  char  volume_path[BUFFER_SIZE];

  if (getuid())
  {
	printf("[PAMela]: You need to be root\n");
	return (-1);
  }
  if (sprintf(volume_path, "%s/.crypted_container", get_user_home(user)) < 0)
	return (-1);
  else if (access(volume_path, F_OK) == -1)
	return (create_container(volume_path, user, pass));
  else if (secure_volume_open(user, pass) != 0)
	return (-1);
  if (execute_cmd("mkdir %s/secure_data-rw", get_user_home(user)) != 0 ||
	  execute_cmd("mount /dev/mapper/%s_container %s/secure_data-rw", user, get_user_home(user)) != 0 ||
	  execute_cmd("chown %s %s/secure_data-rw", user, get_user_home(user)) != 0)
	return (-1);
  return (0);
}
